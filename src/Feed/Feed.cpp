#include "Feed.h"
#include <NetworkConfig.h>
#include <Loop/LoopManager.h>

static const char* tag = "Feed";

#define FRAME_LEN 8
const uint8_t frameStart[FRAME_LEN] = { 0x18, 0x20, 0x55, 0xf2, 0x5a, 0xc0, 0x4d, 0xaa };
const uint8_t frameEnd[FRAME_LEN] = { 0x42, 0x2c, 0xd9, 0xe3, 0xff, 0xa0, 0x11, 0x01 };

Feed::Feed() : buf(ringBufSize), decodeThread("UDPDecodeThread", Feed::decodeFunc, 8192, this){
	LoopManager::addListener(this);
	ESP_LOGI(tag, "connecting to  %s @ port %d\n", batmobileIP.toString().c_str(), port);
	if(!client.connect(batmobileIP, port)){
		ESP_LOGE(tag, "couldnt connect");
		return;
	}
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

	RingBuffer* rbuf = &feed.buf;


	client.onPacket([rbuf](AsyncUDPPacket& packet) {
		rbuf->write(packet.data(), packet.length());
	});

	while(true){

		std::unique_ptr<DriveInfo> frame;
		while(findFrame(feed.buf, frame)){
			vTaskDelay(10);
		}

		feed.mutex.lock();

		if(!frame){
			Serial.println("bad frame");
			feed.mutex.unlock();
			continue;
		}

		if(!feed.processFrame(*frame, feed.doubleBuffer.getWrite()->frame.data())){
			feed.mutex.unlock();
			continue;
		}

		feed.doubleBuffer.getWrite()->driveInfo = *frame;
		feed.doubleBuffer.swap();

		feed.imgReady = true;

		feed.mutex.unlock();

		vTaskDelay(1);
	}

	client.close();
}

void Feed::loop(uint micros){
	if(!imgReady) return;

	mutex.lock();
	if(func){
		func(doubleBuffer.getRead()->driveInfo, doubleBuffer.getRead()->frame.data());
	}
	mutex.unlock();

	imgReady = false;
}

bool Feed::findFrame(RingBuffer& buf, std::unique_ptr<DriveInfo>& out){
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

		//when found, exit while and start filling buffers
		if(frameStartMatch == FRAME_LEN){

			inPacket = true;
			frameStartMatch = 0;
			frameEndMatch = 0;
			break;
		}
	}

	//no start found yet
	if(!inPacket || buf.readAvailable() < FRAME_LEN) return false;

	buf.skip(start - (FRAME_LEN - 1)); //clear bytes before start

	size_t trailer = 0;
	for(trailer = 0; trailer < buf.readAvailable(); trailer++){
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
		buf.skip(start + 1); //clear bytes before data

		size_t driveDataLen = trailer - FRAME_LEN + 1;

		out = DriveInfo::deserialize(buf, driveDataLen);

		if(!out) return false;

		return true;
	}

	ESP_LOGE(tag, "Dropped packet (no trailer found)");
	return false;
}

bool Feed::processFrame(DriveInfo& info, Color* dest){
	if(dest == nullptr){
		Serial.println("processFrame dest nullptr");
		return false;
	}
	jpeg.setUserPointer(dest);
	Serial.println("user potr set");
	delay(50);

	Serial.printf("info frame size: %d\n", info.frame.size);
	delay(60);
	int val = jpeg.openRAM((uint8_t*) (info.frame.data), info.frame.size, [](JPEGDRAW* pDraw) -> int{
		memcpy(pDraw->pUser, pDraw->pPixels, 160 * 120 * 2);
		return 1;
	});


	if(!val){
		ESP_LOGE(tag, "openRAM error: %d\n", jpeg.getLastError());
		return false;
	}

	val = jpeg.decode(0, 0, 0);
	if(!val){
		ESP_LOGE(tag, "decode error: %d\n", jpeg.getLastError());
		return false;
	}

	return true;
}
