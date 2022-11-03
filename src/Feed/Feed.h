#ifndef BATCONTROLLER_FIRMWARE_FEED_H
#define BATCONTROLLER_FIRMWARE_FEED_H

#include <functional>
#include <Loop/LoopListener.h>
#include <AsyncUDP.h>
#include <DriveInfo.h>
#include <Util/Settings.h>
#include <Buffer/RingBuffer.h>
#include <Util/Task.h>
#include "DoubleBuffer.h"
#include <Sync/Mutex.h>
#include <JPEGDEC.h>
#include <Sync/BinarySemaphore.h>

class Feed : private LoopListener {
public:
	Feed();
	~Feed() override;

	/**
	 * Sets function to be executed on Core #1 (Arduino core) when a frame has been received and decoded.
	 * @param func Function to be executed on frame, receives DriveInfo and Color array for decoded frame.
	 */
	void onFrame(std::function<void(const DriveInfo&, const Color* frame)> callback);

private:
	JPEGDEC jpeg;
	AsyncUDP udp;

	RingBuffer rxBuf;
	Mutex rxMut;

	struct {
		DriveInfo info;
		Color* img;
	} frame;

	BinarySemaphore frameConsumed;
	bool frameReady = false;

	// Core 0
	void loop(uint micros) override;

	// Core 1
	Task decodeTask;
	void decodeFunc();

	constexpr static size_t JpgMaxSize = 4500; //upper size limit for JPG quality 30 on Batmobile camera, approximately
	constexpr static size_t RxBufSize = 3 * (sizeof(DriveInfo) + JpgMaxSize);

	std::function<void(const DriveInfo&, const Color* frame)> frameCallback;

	bool findFrame(bool keepLock = false);
};

#endif //BATCONTROLLER_FIRMWARE_FEED_H
