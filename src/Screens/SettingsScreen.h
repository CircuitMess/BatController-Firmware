#ifndef BATCONTROLLER_FIRMWARE_SETTINGSSCREEN_H
#define BATCONTROLLER_FIRMWARE_SETTINGSSCREEN_H

#include "../Interface/LVScreen.h"
#include <Input/InputListener.h>
#include <DisconnectListener.h>
#include "../Elements/Prompt.h"

class SettingsScreen : public LVScreen, private InputListener, private DisconnectListener {
public:
	SettingsScreen();

	virtual ~SettingsScreen();

	void onStarting() override;

	void onStop() override;
	void onStart() override;

private:
	lv_obj_t* flexContainer;

	lv_obj_t* sound;
	lv_obj_t* shutdownTime;
	lv_obj_t* screenBrightness;
	lv_obj_t* factoryReset;
	Prompt factoryResetPrompt;
	lv_obj_t* title;
	lv_obj_t* pairBatmobile;
	lv_obj_t* saveBtn;

	lv_obj_t* soundSlider;
	lv_obj_t* shutdownTimeLabel;
	lv_obj_t* shutdownSlider;
	lv_obj_t* brightnessSlider;

	lv_style_t style_knob;
	lv_style_t style_def;
	lv_style_t style_focused;
	lv_style_t style_main;
	lv_style_t style_pressed;

	void backToMain();

	void onDisconnected() override;
};

#endif //BATCONTROLLER_FIRMWARE_SETTINGSSCREEN_H
