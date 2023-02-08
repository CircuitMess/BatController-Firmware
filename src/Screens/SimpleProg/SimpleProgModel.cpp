#include "SimpleProgModel.h"
#include <font/lv_symbol_def.h>

const char* DirectionSymbols[4] = { LV_SYMBOL_UP, LV_SYMBOL_DOWN, LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT };
const uint8_t SoundsNum = 14;
const char* SoundNames[SoundsNum] = {
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
const uint8_t ColorsNum = 8;
const char* ColorNames[ColorsNum] = {
		"Off",
		"Red",
		"Green",
		"Yellow",
		"Blue",
		"Magenta",
		"Cyan",
		"White",
};