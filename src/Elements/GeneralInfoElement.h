#ifndef BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H
#define BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H

#include "../Interface/LVObject.h"
#include <DriveMode.h>
#include <Loop/LoopListener.h>
#include "BatteryElement.h"
#include "SignalElement.h"

class GeneralInfoElement : public LVObject, private LoopListener, private ComListener {
public:
	GeneralInfoElement(lv_obj_t* parent, DriveMode mode = DriveMode::Idle);
	~GeneralInfoElement();
	void setMode(DriveMode mode);

private:
	void onBattery(uint8_t percent, bool charging) override;
	void loop(uint micros) override;

	lv_obj_t* modeImg;
	SignalElement* signal;
	BatteryElement* conBat;
	BatteryElement* mobBat;

	uint32_t counter = 0;
	static constexpr uint32_t BatteryCheckInterval = 1000000;
};


#endif //BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H
