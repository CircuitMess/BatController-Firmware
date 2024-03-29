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

	frame.img = static_cast<Color*>(heap_caps_malloc(160 * 120 * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_32BIT));

	udp.listen(controllerIP, feedPort);
	udp.writeTo((const uint8_t*)tag, 1, batmobileIP, feedPort);
	udp.onPacket([this](AsyncUDPPacket& packet){
		Locker lock(rxMut);

		if(rxBuf.writeAvailable() < packet.length()){
			ESP_LOGD(tag, "RX buffer full, clearing");
			rxBuf.clear();
		}

		rxBuf.write(packet.data(), packet.length());
	});

	LoopManager::addListener(this);
	taskKill = false;
	frameConsumed.signal();
	decodeTask.start(1, 0);
}

Feed::~Feed(){
	taskKill = true;
	frameConsumed.signal();
	decodeTask.stop(true);
	LoopManager::removeListener(this);
	free(frame.img);

	udp.onPacket(AuPacketHandlerFunction());
	udp.close();
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
	if(bytesMatched != sizeof(FrameHeader)){
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

	// Read shifted frame size
	uint8_t frameShiftedSizeRaw[4];
	for(int i = 0; i < 4; i++){
		frameShiftedSizeRaw[i] = *rxBuf.peek<uint8_t>(sizeof(FrameHeader) + sizeof(size_t) + i);
	}

	// Clear buffer if shifted size doesn't match size when shifted
	for(int i = 0; i < 4; i++){
		if(frameShiftedSizeRaw[FrameSizeShift[i]] != frameSizeRaw[i]){
			ESP_LOGD(tag, "Frame checksum doesn't match");
			rxBuf.skip(sizeof(FrameHeader));
			return false;
		}
	}

	// Abort if rest of frame is missing
	if(rxBuf.readAvailable() < frameSize + sizeof(FrameHeader) + sizeof(FrameTrailer) + sizeof(size_t) * 2){
		return false;
	}

	// Search for frame trailer
	size_t endOffset = frameSize + sizeof(FrameHeader) + sizeof(size_t) * 2;
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

void Feed::onFrame(std::function<void(std::shared_ptr<const DriveInfo>, const Color*)> callback){
	this->frameCallback = callback;
}

void Feed::decodeFunc(){
	while(decodeTask.running){
		frameConsumed.wait();

		if(taskKill) return;

		/** We can't continue into the next iteration in case of error, because the task will end up waiting for a
		 * semaphore that will never get signaled - frameReady never gets set, and the main thread never signals
		 * the semaphore. Therefore, a goto is required so that once the semaphore is passed, it isn't waited on again
		 * unless a frame has been produced.
		 */

start:
		if(!findFrame(true)){
			rxMut.unlock();
			delay(10);

			if(taskKill) return;

			goto start;
		}

		rxBuf.skip(sizeof(FrameHeader));
		size_t size;
		rxBuf.read(reinterpret_cast<uint8_t*>(&size), sizeof(size_t));
		rxBuf.skip(sizeof(size_t));

		size_t available = rxBuf.readAvailable();
		std::shared_ptr<DriveInfo> frame = DriveInfo::deserialize(rxBuf, size);

		size_t readTotal = available - rxBuf.readAvailable();
		rxBuf.skip(size - readTotal); // skip frame if deserialize exited early

		rxBuf.skip(sizeof(FrameTrailer));

		rxMut.unlock();

		if(taskKill){
			free(frame->frame.data);
			frame->frame.data = nullptr;
			return;
		}

		const auto frameDone = [this, frame](){
			if(frame){
				free(frame->frame.data);
				frame->frame.data = nullptr;
			}

			this->frame.info = frame;
			if(this->frame.info){
				this->frame.info->frame = {};
			}

			if(postProcCallback){
				postProcCallback(this->frame.info ? *this->frame.info : DriveInfo(), this->frame.img);
			}

			frameReady = true;
		};

		const auto handleError = [this, frameDone](){
			memset(this->frame.img, 0, 160*120*2);
			frameDone();
		};

		if(frame == nullptr){
			ESP_LOGD(tag, "Couldn't deserialize frame");

			handleError();
			if(taskKill) return;
			continue;
		}

		if(frame->frame.size == 0 || frame->frame.data == nullptr){
			ESP_LOGE(tag, "JPG frame size: %zu, ptr: 5x%p", frame->frame.size, frame->frame.data);

			handleError();
			if(taskKill) return;
			continue;
		}

		jpeg.openRAM((uint8_t*) (frame->frame.data), frame->frame.size, [](JPEGDRAW* data) -> int{
			for(int y = data->y, iy = 0; y < data->y + data->iHeight; y++, iy++){
				size_t offset = y * 160 + data->x;
				size_t ioffset = iy * data->iWidth;
				memcpy((uint8_t*) data->pUser + offset * 2, (uint8_t*) data->pPixels + ioffset * 2, data->iWidth * 2);
			}
			return 1;
		});

		jpeg.setUserPointer(this->frame.img);
		jpeg.setPixelType(RGB565_LITTLE_ENDIAN);

		if(jpeg.decode(0, 0, 0) == 0){
			ESP_LOGE(tag, "decode error: %d", jpeg.getLastError());

			handleError();
			if(taskKill) return;
			continue;
		}

		frameDone();
	}
}

void Feed::loop(uint micros){
	if(!frameReady) return;

	if(frameCallback){
		frameCallback(frame.info, frame.img);
		frame.info.reset();
	}

	frameReady = false;
	frameConsumed.signal();
}

void Feed::setPostProcCallback(std::function<void(const DriveInfo&, Color*)> callback){
	postProcCallback = callback;
}
