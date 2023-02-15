#ifndef BATCONTROLLER_FIRMWARE_SHUTDOWNSERVICE_H
#define BATCONTROLLER_FIRMWARE_SHUTDOWNSERVICE_H

#include <Settings.h>
#include <Input/InputListener.h>
#include <Loop/LoopListener.h>

class ShutdownService : private InputListener, private LoopListener {
public:
	void begin();
	void pause();
	void resume();
	/**
	 * Call when an activity other than keypresses have occured.
	 * This will reset the inactivity timer.
	 */
	void activityReset();

private:
	void loop(uint micros) override;

	void anyKeyPressed() override;

	uint32_t timer = 0;

	bool paused = false;

	bool done = false;
	void shutdown();
};

extern ShutdownService AutoShutdown;

#endif //BATCONTROLLER_FIRMWARE_SHUTDOWNSERVICE_H
