#ifndef BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H
#define BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H

#include "../Interface/LVObject.h"
#include <DriveMode.h>
#include "BatteryElement.h"
#include "SignalElement.h"

class GeneralInfoElement : public LVObject {
public:
	GeneralInfoElement(lv_obj_t* parent, DriveMode mode = DriveMode::Idle);
	void setMode(DriveMode mode);
	void setSignal(uint8_t index); //0-3
	void setConBat(uint8_t index); //0-7
	void setMobBat(uint8_t index); //0-7

private:
	lv_obj_t* modeImg;
	SignalElement* signal;
	BatteryElement* conBat;
	BatteryElement* mobBat;

	static const char* modePaths[5];
};


#endif //BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H
