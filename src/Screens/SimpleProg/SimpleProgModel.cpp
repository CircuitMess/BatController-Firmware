#include "SimpleProgModel.h"
#include <font/lv_symbol_def.h>

const char* DirectionSymbols[4] = { LV_SYMBOL_UP, LV_SYMBOL_DOWN, LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT };
const uint8_t SoundsNum = 25;
const char* SoundNames[SoundsNum] = {
		"bats",
		"beep",
		"boostOff",
		"boostOn",
		"boostTurn",
		"booted",
		"disconnect",
		"driverExit",
		"driverStart",
		"error",
		"explosion",
		"honk0",
		"honk1",
		"honk2",
		"honk3",
		"honk4",
		"honk5",
		"honk6",
		"idle1",
		"idle2",
		"idle3",
		"idle4",
		"off",
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