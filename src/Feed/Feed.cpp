#include "Feed.h"
#include <NetworkConfig.h>
#include <Loop/LoopManager.h>

static const char* tag = "Feed";

#define FRAME_LEN 8
const uint8_t frameStart[FRAME_LEN] = { 0x18, 0x20, 0x55, 0xf2, 0x5a, 0xc0, 0x4d, 0xaa };
const uint8_t frameEnd[FRAME_LEN] = { 0x42, 0x2c, 0xd9, 0xe3, 0xff, 0xa0, 0x11, 0x01 };

Feed::Feed() : buf(ringBufSize), decodeThread("UDPDecodeThread", Feed::decodeFunc, 8192, this){
	LoopManager::addListener(this);
	ESP_LOGI(tag, "connecting to  %s @ port %d", batmobileIP.toString().c_str(), port);
	if(!client.connect(batmobileIP, port)){
		ESP_LOGE(tag, "couldnt connect");
		return;
	}
	while(!client.connected());
	while(!client.listen(port));

	ESP_LOGI(tag, "connected");

	client.onPacket([this](AsyncUDPPacket& packet) {
		ringBufMut.lock();
		if(buf.writeAvailable() < packet.length()) buf.clear();
		buf.write(packet.data(), packet.length());
		ringBufMut.unlock();
	});

	decodeThread.start(5, 0);
}

Feed::~Feed(){
	decodeThread.stop(true);
	LoopManager::removeListener(this);
}


void Feed::onFrame(std::function<void(const DriveInfo&, const Color* frame)> func){
	this->func = func;
}

void Feed::decodeFunc(Task* t){
	Feed &feed = *(Feed*) t->arg;
	auto& client = feed.client;

	while(true){

		std::unique_ptr<DriveInfo> frame;

		feed.ringBufMut.lock();
		while(!findFrame(feed.buf, frame)){
			feed.ringBufMut.unlock();
			vTaskDelay(2);
			feed.ringBufMut.lock();
		}
		ESP_LOGI(tag, "found good frame");

		//frame found, try to find additional frames afterwards
		while(findFrame(feed.buf, frame)){
			feed.ringBufMut.unlock();
			vTaskDelay(2);
			feed.ringBufMut.lock();
		}
		feed.ringBufMut.unlock();

		ESP_LOGI(tag, "found latest good frame");



		feed.doubleBufMut.lock();

		if(!frame){
			feed.doubleBufMut.unlock();
			continue;
		}

		if(!feed.processFrame(*frame, feed.doubleBuffer.getWrite()->frame.data())){
			feed.doubleBufMut.unlock();
			continue;
		}

		feed.doubleBuffer.getWrite()->driveInfo = *frame;
		feed.doubleBuffer.swap();

		feed.imgReady = true;

		feed.doubleBufMut.unlock();

		vTaskDelay(1);
	}

	client.close();
}

void Feed::loop(uint micros){
	if(!imgReady) return;

	doubleBufMut.lock();
	if(func){
		func(doubleBuffer.getRead()->driveInfo, doubleBuffer.getRead()->frame.data());
	}
	doubleBufMut.unlock();

	imgReady = false;
}

bool Feed::findFrame(RingBuffer& buf, std::unique_ptr<DriveInfo>& out){
	if(!buf.readAvailable()){
		ESP_LOGW(tag, "No read buffer available");
		return false;
	}

	bool inPacket = false;
	uint8_t frameStartMatch = 0;
	uint8_t frameEndMatch = 0;
	uint8_t c;

	//scrubbing through read data
	size_t start = 0;
	for(start = 0; start < buf.readAvailable(); start++){
		//search for frameStart

		if(*buf.peek<uint8_t>(start) == frameStart[frameStartMatch]){
			frameStartMatch++;
		}else{
			frameStartMatch = 0;
		}

		if(frameStartMatch == FRAME_LEN){
			inPacket = true;
			frameStartMatch = 0;
			break;
		}
	}

	//no start found yet
	if(!inPacket){
		ESP_LOGI(tag, "Packet header not found yet");
		return false;
	}
	if(buf.readAvailable() < FRAME_LEN){
		ESP_LOGI(tag, "Nothing to read after packet header");
		return false;
	}

	buf.skip(start + 1 - FRAME_LEN); //skip bytes before header
	size_t trailer = FRAME_LEN; //skip header when peeking for trailer
	ESP_LOGI(tag, "data after header: %zu", buf.readAvailable());
	for(; trailer < buf.readAvailable(); trailer++){
		//search for frameEnd
		if(*buf.peek<char>(trailer) == frameEnd[frameEndMatch]){
			frameEndMatch++;
		}else{
			frameEndMatch = 0;
		}

		if(frameEndMatch == FRAME_LEN){
			inPacket = false;
			frameStartMatch = 0;
			frameEndMatch = 0;
			break;
		}
	}


	if(!inPacket){
		buf.skip(FRAME_LEN); //clear header bytes before data


		size_t driveDataLen = trailer - 2*FRAME_LEN + 1;
		ESP_LOGI(tag, "DriveDataLen: %zu", driveDataLen);
		out = DriveInfo::deserialize(buf, driveDataLen);

		if(!out){
			ESP_LOGE(tag, "DriveInfo deserialization error");
			return false;
		}

		return true;
	}

	ESP_LOGW(tag, "Dropped packet (no trailer found)");
	return false;
}

bool Feed::processFrame(DriveInfo& info, Color* dest){
	if(dest == nullptr){
		ESP_LOGE(tag, "Double buffer mem error");
		return false;
	}
	jpeg.setUserPointer(dest);

	int val = jpeg.openRAM((uint8_t*) (info.frame.data), info.frame.size, [](JPEGDRAW* pDraw) -> int{
		memcpy(pDraw->pUser, pDraw->pPixels, 160 * 120 * 2);
		return 1;
	});


	if(!val){
		ESP_LOGE(tag, "openRAM error: %d", jpeg.getLastError());
		return false;
	}

	val = jpeg.decode(0, 0, 0);
	if(!val){
		ESP_LOGE(tag, "decode error: %d", jpeg.getLastError());
		return false;
	}

	return true;
}
