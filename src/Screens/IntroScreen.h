#ifndef BATCONTROLLER_FIRMWARE_INTROSCREEN_H
#define BATCONTROLLER_FIRMWARE_INTROSCREEN_H

#include "../Interface/LVScreen.h"
#include <Loop/LoopListener.h>

class IntroScreen : public LVScreen, private LoopListener {
public:
	IntroScreen(void (* callback)());

	void onStart() override;
	void onStop() override;

private:
	void (*callback)() = nullptr;

	void loop(uint micros) override;

	uint32_t currentFrame = 0;
	uint32_t frameTime;

	lv_obj_t* img;

};


#endif //BATCONTROLLER_FIRMWARE_INTROSCREEN_H
