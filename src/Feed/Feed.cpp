#include "Feed.h"
#include <NetworkConfig.h>
#include <Loop/LoopManager.h>

static const char* tag = "Feed";

class Locker {
public:
	explicit Locker(Mutex& mut) : mut(mut){
		mut.lock();
	}

	virtual ~Locker(){
		if(keep) return;
		mut.unlock();
	}

	void setKeep(bool keep){
		Locker::keep = keep;
	}

private:
	Mutex& mut;
	bool keep = false;

};

Feed::Feed() : rxBuf(RxBufSize), decodeTask("Feed", [](Task* t){
	auto feed = static_cast<Feed*>(t->arg);
	feed->decodeFunc();
}, 8192, this){

	frame.img = static_cast<Color*>(malloc(160 * 120 * 2));

	udp.listen(controllerIP, feedPort);
	udp.onPacket([this](AsyncUDPPacket& packet) {
		Locker lock(rxMut);

		if(rxBuf.writeAvailable() < packet.length()){
			ESP_LOGD(tag, "RX buffer full, clearing");
			rxBuf.clear();
		}

		rxBuf.write(packet.data(), packet.length());
	});

	LoopManager::addListener(this);
	frameConsumed.signal();
	decodeTask.start(1, 0);
}

Feed::~Feed(){
	decodeTask.stop(true);
	LoopManager::removeListener(this);
	free(frame.img);
}

bool Feed::findFrame(bool keepLock){
	Locker lock(rxMut);
	if(rxBuf.readAvailable() < sizeof(FrameHeader)) return false;


	// Search for frame header
	size_t bytesRead = 0;
	size_t bytesMatched = 0;
	for(; bytesRead < rxBuf.readAvailable(); bytesRead++){
		uint8_t byte = *rxBuf.peek<uint8_t>(bytesRead);

		if(byte == FrameHeader[bytesMatched]){
			bytesMatched++;
			if(bytesMatched == sizeof(FrameHeader)){
				bytesRead++;
				break;
			}
		}else{
			bytesMatched = 0;
		}
	}

	// Clear buffer if header isn't found
	if(bytesMatched == 0){
		size_t size = rxBuf.readAvailable();
		rxBuf.clear();
		ESP_LOGD(tag, "Couldn't find frame header. Skipping %zu bytes", size);
		return false;
	}

	// Skip bytes preceding header
	if(bytesRead - bytesMatched != 0){
		rxBuf.skip(bytesRead - bytesMatched);
		ESP_LOGD(tag, "Found header. Skipping %zu bytes", bytesRead - bytesMatched);
	}

	// Abort if rest of frame is missing
	if(rxBuf.readAvailable() < 4){
		return false;
	}

	// Read frame size
	uint8_t frameSizeRaw[4];
	for(int i = 0; i < 4; i++){
		frameSizeRaw[i] = *rxBuf.peek<uint8_t>(sizeof(FrameHeader) + i);
	}
	size_t frameSize = *((size_t*) frameSizeRaw);


	// Clear buffer if frame is malformed
	if(frameSize >= RxBufSize){
		ESP_LOGD(tag, "Frame size yuge: %zu", frameSize);
		rxBuf.clear();
		return false;
	}

	// Abort if rest of frame is missing
	if(rxBuf.readAvailable() < frameSize + sizeof(FrameHeader) + sizeof(FrameTrailer) + sizeof(size_t)){
		return false;
	}

	// Search for frame trailer
	size_t endOffset = frameSize + sizeof(FrameHeader) + sizeof(size_t);
	for(bytesMatched = 0; bytesMatched < sizeof(FrameTrailer); bytesMatched++){
		uint8_t byte = *rxBuf.peek<uint8_t>(endOffset + bytesMatched);
		if(byte != FrameTrailer[bytesMatched]) break;
	}

	// Clear whole frame if trailer isn't found at expected offset
	if(bytesMatched != sizeof(FrameTrailer)){
		rxBuf.skip(endOffset + sizeof(FrameTrailer));
		ESP_LOGD(tag, "Trailer missmatch. Clearing %zu bytes", endOffset + sizeof(FrameTrailer));
		return false;
	}

	lock.setKeep(keepLock);
	return true;
}

void Feed::onFrame(std::function<void(const DriveInfo&, const Color* frame)> callback){
	this->frameCallback = callback;
}

void Feed::decodeFunc(){
	while(decodeTask.running){
		frameConsumed.wait();

		/** We can't continue into the next iteration in case of error, because the task will end up waiting for a
		 * semaphore that will never get signaled - frameReady never gets set, and the main thread never signals
		 * the semaphore. Therefore, a goto is required.
		 */

start:
		if(!findFrame(true)){
			rxMut.unlock();
			delay(10);

			goto start;
		}

		rxBuf.skip(sizeof(FrameHeader));
		size_t size;
		rxBuf.read(reinterpret_cast<uint8_t*>(&size), sizeof(size_t));
		auto frame = DriveInfo::deserialize(rxBuf, size);
		rxBuf.skip(sizeof(FrameTrailer));

		rxMut.unlock();

		if(frame == nullptr){
			ESP_LOGD(tag, "Couldn't deserialize frame");
			goto start;
		}

		jpeg.openRAM((uint8_t*) (frame->frame.data), frame->frame.size, [](JPEGDRAW* data) -> int{
			size_t offset = (data->x + data->y * 160) * (data->iBpp / 8);
			memcpy((uint8_t*) data->pUser + offset, data->pPixels, data->iWidth * data->iHeight * (data->iBpp / 8));
			return 1;
		});

		jpeg.setUserPointer(this->frame.img);
		jpeg.setPixelType(RGB565_LITTLE_ENDIAN);

		if(jpeg.decode(0, 0, 0) == 0){
			ESP_LOGE(tag, "decode error: %d", jpeg.getLastError());
			goto start;
		}

		this->frame.info = *frame;
		this->frame.info.frame = {};

		frameReady = true;
	}
}

void Feed::loop(uint micros){
	if(!frameReady) return;

	if(frameCallback){
		frameCallback(frame.info, frame.img);
	}

	frameReady = false;
	frameConsumed.signal();
}
