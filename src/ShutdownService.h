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

private:
	void loop(uint micros) override;

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	void shutdown();
	uint32_t timer = 0;

	bool paused = false;
};

extern ShutdownService AutoShutdown;

#endif //BATCONTROLLER_FIRMWARE_SHUTDOWNSERVICE_H
