#include "SimpleProgModel.h"
#include <font/lv_symbol_def.h>

const char* DirectionSymbols[4] = { LV_SYMBOL_UP, LV_SYMBOL_DOWN, LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT };
const uint8_t SoundsNum = 25;
const char* SoundNames[SoundsNum] = {
		"bats",
		"beep",
		"boost off",
		"boost on",
		"boost turn",
		"booted",
		"disconnect",
		"driver exit",
		"driver enter",
		"error",
		"explosion",
		"honk 1",
		"honk 2",
		"honk 3",
		"honk 4",
		"honk 5",
		"honk 6",
		"honk 7",
		"idle 1",
		"idle 2",
		"idle 3",
		"idle 4",
		"turning off",
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