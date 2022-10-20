#include "Feed.h"
#include "../NetworkConfig.h"
#include <Loop/LoopManager.h>

const char* tag = "Feed";
#define FRAME_LEN 8
const uint8_t frameStart[FRAME_LEN] = { 0x18, 0x20, 0x55, 0xf2, 0x5a, 0xc0, 0x4d, 0xaa };
const uint8_t frameEnd[FRAME_LEN] = { 0x42, 0x2c, 0xd9, 0xe3, 0xff, 0xa0, 0x11, 0x01 };

Feed::Feed() : buf(ringBufSize), decodeThread("UDPDecodeThread", Feed::decodeFunc, 4096, this){
	LoopManager::addListener(this);
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
	auto feed = *(Feed*)t->arg;
	auto& client = feed.client;

	ESP_LOGI(tag, "connecting to  %s @ port %d\n", batmobileIP.toString().c_str(), port);
	if(!client.connect(batmobileIP, port)){
		ESP_LOGE(tag, "couldnt connect");
		return;
	}

	client.onPacket([&feed](AsyncUDPPacket& packet){
		feed.buf.write(packet.data(), packet.length());
	});

	DriveInfo frame{};

	while(true){

		bool found = false;
		while(findFrame(feed.buf, frame));
		feed.buf.clear();

		feed.mutex.lock();

		feed.processFrame(frame, feed.doubleBuffer.getWrite()->frame.data());
		feed.doubleBuffer.getWrite()->driveInfo = frame;
		feed.doubleBuffer.swap();

		feed.mutex.unlock();
	}

	client.close();
}

void Feed::loop(uint micros){
	if(!imgReady) return;

	mutex.lock();
	func(doubleBuffer.getRead()->driveInfo, doubleBuffer.getRead()->frame.data());
	mutex.unlock();

	imgReady = false;
}

bool Feed::findFrame(RingBuffer& buf, DriveInfo& out){
	bool inPacket = false;
	uint8_t frameStartMatch = 0;
	uint8_t frameEndMatch = 0;
	uint8_t c;
	//scrubbing through read data
	while(buf.readAvailable()){
		//search for frameStart
		buf.read(&c, 1);
		if(c == frameStart[frameStartMatch]){
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
	if(!inPacket || buf.readAvailable() < 8) return false;


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
		trailer -= 8; //get start of trailer (length of DriveInfo data)

		//TODO - create DriveInfo out using DriveInfo constructor, length of entire data is contained in <trailer:int>
		return true;
	}

	ESP_LOGE(tag, "Dropped packet (no trailer found)");
	return false;
}

void Feed::processFrame(DriveInfo& info, Color* dest){
	int val = jpeg.openRAM(const_cast<uint8_t*>(info.jpg), info.jpgSize, [](JPEGDRAW* pDraw)->int{
		memcpy(pDraw->pUser, pDraw->pPixels, 160*120*2);
		return 1;
	});
	if(!val){
		ESP_LOGE(tag, "openRAM error: %d\n", jpeg.getLastError());
	}

	jpeg.setUserPointer(dest);
	val = jpeg.decode(0, 0, 0);
	if(!val){
		ESP_LOGE(tag, "decode error: %d\n", jpeg.getLastError());
	}


}
