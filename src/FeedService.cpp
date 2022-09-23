#include "FeedService.h"
#include "NetworkConfig.h"

#define FRAME_LEN 8

const uint8_t frameStart[FRAME_LEN] = { 0x18, 0x20, 0x55, 0xf2, 0x5a, 0xc0, 0x4d, 0xaa };
const uint8_t frameEnd[FRAME_LEN] = { 0x42, 0x2c, 0xd9, 0xe3, 0xff, 0xa0, 0x11, 0x01 };
const uint8_t frameAck[FRAME_LEN] = { 0xba, 0x14, 0x05, 0xb0, 0x21, 0xa2, 0x03, 0x83 };

FeedService Feed;

bool FeedService::connect(){
	state = Connecting;


	client.onConnect([this](void*, AsyncClient* server){
		state = Connected;
		Serial.println("connected to port");
		//send ack after receiving frame
		server->write(reinterpret_cast<const char*>(frameAck), FRAME_LEN);
	}, nullptr);

	client.onDisconnect([this](void*, AsyncClient* server){
		//TODO - reconnect to wifi in case of total disconnect
		state = Connecting;
		if(camFrameBuf){
			delete camFrameBuf;
			camFrameBuf = nullptr;
		}
		Serial.println("disconnected from port");
	});

	client.onData([this](void* arg, AsyncClient* server, void* data, size_t len){

		bool frameRead = processData((uint8_t*)data, len);

		//send ack after receiving frame
		server->write(reinterpret_cast<const char*>(frameAck), FRAME_LEN);

		if(frameRead){
			if(camFrame.jpgSize >= jpgMaxSize){
				Serial.printf("jpgSize: %d, max size: %d\n", camFrame.jpgSize, jpgMaxSize);
				Serial.println("insufficient buffer size");
			}
			func(camFrame);
		}
	}, nullptr);

	client.onError([this](void*, AsyncClient* c, err_t error){
		state = Connecting;
		if(camFrameBuf){
			delete camFrameBuf;
			camFrameBuf = nullptr;
		}
		Serial.print("error occured: ");
		Serial.println(c->errorToString(error));
		connect();
	}, nullptr);


	client.onTimeout([this](void*, AsyncClient*, uint32_t time){
		state = Connecting;
		if(camFrameBuf){
			delete camFrameBuf;
			camFrameBuf = nullptr;
		}
		Serial.printf("timeout error %d passed\n", time);
	}, nullptr);

	Serial.printf("connecting to  %s @ port %d\n", batmobileIP.toString().c_str(), port);
	bool c = client.connect(batmobileIP, port);
	if(!c){
		Serial.println("couldnt connect");
	}

	return c;
}

FeedService::State FeedService::getState(){
	return state;
}

void FeedService::onFrame(std::function<void(const CamFrame&)> func){
	this->func = func;
}

void FeedService::disconnect(){
	state = Idle;

	if(camFrameBuf){
		delete camFrameBuf;
		camFrameBuf = nullptr;
	}

	client.close();
}


bool FeedService::processData(const uint8_t* data, size_t len){
	size_t cursor = 0;

	//scrubbing through read data
	while(cursor < len && !inPacket){
		//search for frameStart

		if(data[cursor] == frameStart[frameStartMatch]){
			frameStartMatch++;
		}else{
			frameStartMatch = 0;
		}

		cursor++;

		//when found, exit while and start filling buffers
		if(frameStartMatch == FRAME_LEN){

			if(camFrameBuf == nullptr){
				camFrameBuf = new DataBuffer(sizeof(CamFrame) - sizeof(CamFrame::jpg) + jpgMaxSize);
			}
			inPacket = true;
			frameStartMatch = 0;
			frameEndMatch = 0;
			break;
		}
	}


	//no start found yet
	if(!inPacket) return false;


	//search for frameEnd
	size_t endCursor = cursor;

	while(endCursor < len && inPacket){
		//search for frameEnd

		if(data[endCursor] == frameEnd[frameEndMatch]){
			frameEndMatch++;
		}else{
			frameEndMatch = 0;
		}

		endCursor++;

		if(frameEndMatch == FRAME_LEN){
			inPacket = false;
			frameStartMatch = 0;
			frameEndMatch = 0;
			break;
		}
	}

	size_t written = min(endCursor - cursor, camFrameBuf->writeAvailable());
	memcpy(camFrameBuf->writeData(), data + cursor, written);
	camFrameBuf->writeMove(written);


	//exiting packet, wrapup everything and call the callback func
	if(!inPacket){
		memcpy(&camFrame.jpgSize, camFrameBuf->readData(), sizeof(camFrame.jpgSize));
		camFrameBuf->readMove(sizeof(camFrame.jpgSize));


		camFrame.jpg = camFrameBuf->readData();
		camFrameBuf->readMove(camFrame.jpgSize);

		size_t restSize = sizeof(camFrame) - sizeof(camFrame.jpgSize) - sizeof(camFrame.jpg);
		memcpy(&camFrame + sizeof(camFrame.jpgSize) + sizeof(camFrame.jpg), camFrameBuf->readData(), restSize);

		camFrameBuf->clear();

		return true;
	}

	return false;
}


