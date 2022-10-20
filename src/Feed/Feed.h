#ifndef BATCONTROLLER_FIRMWARE_FEED_H
#define BATCONTROLLER_FIRMWARE_FEED_H

#include <functional>
#include <Loop/LoopListener.h>
#include <AsyncUDP.h>
#include "../Model.hpp"
#include <Util/Settings.h>
#include <Buffer/RingBuffer.h>
#include <Util/Task.h>
#include "DoubleBuffer.h"
#include <Sync/Mutex.h>
#include <JPEGDEC.h>

class Feed : private LoopListener {
public:
	Feed();
	~Feed() override;

	/**
	 * Sets function to be executed on Core #1 (Arduino core) when a frame has been received and decoded.
	 * @param func Function to be executed on frame, receives DriveInfo and Color array for decoded frame.
	 */
	void onFrame(std::function<void(const DriveInfo&, const Color* frame)> func);


private:
	struct buffer_t {
		DriveInfo driveInfo;
		std::array<Color, 160 * 120> frame;
	};
	//Double buffer
	DoubleBuffer<buffer_t> doubleBuffer;
	Mutex mutex;


	//Thread on Core 1
	std::function<void(const DriveInfo&, const Color* frame)> func;
	void loop(uint micros) override;
	AsyncUDP client;
	bool imgReady = false;


	//Thread on Core 0
	Task decodeThread;
	static void decodeFunc(Task* t);
	void processFrame(DriveInfo& info, Color* dest);
	static bool findFrame(RingBuffer& buf, DriveInfo& out);

	RingBuffer buf;
	JPEGDEC jpeg;

	//constants
	constexpr static size_t jpgMaxSize = 3000; //upper size limit for JPG quality 30 on Batmobile camera, approximately
	constexpr static size_t ringBufSize = 3 * (sizeof(DriveInfo) + jpgMaxSize);
	constexpr static uint16_t port = 5000;
};

#endif //BATCONTROLLER_FIRMWARE_FEED_H
