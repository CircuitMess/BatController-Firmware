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

	static const char* modePaths[7];

	uint32_t counter = 0;
	static constexpr uint32_t BatteryCheckInterval = 1000000;
	uint8_t conBatIndex;
};


#endif //BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H
