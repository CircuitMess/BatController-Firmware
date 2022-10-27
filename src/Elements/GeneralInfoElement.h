#ifndef BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H
#define BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H

#include "../Interface/LVObject.h"
#include <DriveMode.h>
#include "BatteryElement.h"
#include "SignalElement.h"

class GeneralInfoElement : public LVObject {
public:
	GeneralInfoElement(lv_obj_t* parent, DriveMode mode = Idle);
	void setMode(DriveMode mode);

private:
	lv_obj_t* modeImg;
	SignalElement* signal;
	BatteryElement* conBat;
	BatteryElement* mobBat;
};


#endif //BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H
