#ifndef BATCONTROLLER_FIRMWARE_ACTIONEDITMODAL_H
#define BATCONTROLLER_FIRMWARE_ACTIONEDITMODAL_H

#include "../../Interface/LVModal.h"
#include "SimpleProgModel.h"

class ActionEditModal : public LVModal{
public:
	explicit ActionEditModal(LVScreen* parent);
	void startEdit(Simple::Action& action);
protected:
	void onStart() override;
	void onStop() override;
private:


	static constexpr size_t w = 79;
	static constexpr size_t h = 48;

	static constexpr size_t entryW = 75;
	static constexpr size_t entryH = 13;

	void addDirection(Simple::Action& action);
	void addDuration(Simple::Action& action);
	void addSpeed(Simple::Action& action);
	void addToggle(Simple::Action& action);
	void addSound(Simple::Action& action);
	void addColor(Simple::Action& action);

};

static const char* DirectionSymbols[4] = { LV_SYMBOL_UP, LV_SYMBOL_DOWN, LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT };
static const uint8_t SoundsNum = 14;
static const char* SoundNames[SoundsNum] = {
        "autopilot",
        "autopilotSlow",
        "beep",
        "boostOff",
        "boostOn",
        "boostTurn",
        "disconnect",
        "driverExit",
        "driverStart",
        "honk",
        "manual",
        "manualSlow",
        "paired",
        "scan"
};
static const uint8_t ColorsNum = 5;
static const char* ColorNames[ColorsNum] = {
        "Off", "Red", "Green", "Blue", "White"
};

#endif //BATCONTROLLER_FIRMWARE_ACTIONEDITMODAL_H
