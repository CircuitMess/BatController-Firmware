#ifndef BATCONTROLLER_FIRMWARE_FEEDSERVICE_H
#define BATCONTROLLER_FIRMWARE_FEEDSERVICE_H

#include <functional>
#include <Loop/LoopListener.h>
#include "Model.hpp"
#include <ESPAsyncTCP.h>
#include <Util/Settings.h>
#include <Buffer/DataBuffer.h>


class FeedService{
public:
	bool connect();
	void disconnect();

	enum State {
		Idle, Connecting, Connected
	};

	State getState();

	void onFrame(std::function<void(const CamFrame&)> func);

private:
	State state = Idle;

	AsyncClient client;

	const uint16_t port = 5000;

	std::function<void(const CamFrame&)> func;

	CamFrame camFrame;
	bool inPacket = false;
	uint8_t frameStartMatch = 0;
	uint8_t frameEndMatch = 0;
	DataBuffer *camFrameBuf = nullptr;
	constexpr static size_t jpgMaxSize = 3000;



	bool processData(const uint8_t* data, size_t len);

};

extern FeedService Feed;

#endif //BATCONTROLLER_FIRMWARE_FEEDSERVICE_H
