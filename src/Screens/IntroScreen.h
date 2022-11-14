#ifndef BATCONTROLLER_FIRMWARE_INTROSCREEN_H
#define BATCONTROLLER_FIRMWARE_INTROSCREEN_H

#include "../Interface/LVScreen.h"

class IntroScreen : public LVScreen {
public:
	IntroScreen(void (* callback)());

	void onStart() override;
	void onStop() override;

private:
	lv_obj_t* gif;
	void (*callback)() = nullptr;
};


#endif //BATCONTROLLER_FIRMWARE_INTROSCREEN_H
