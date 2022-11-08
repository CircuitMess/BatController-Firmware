#ifndef BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H
#define BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H

#include "../Interface/LVObject.h"
#include <DriveMode.h>
#include <Loop/LoopListener.h>
#include "BatteryElement.h"
#include "SignalElement.h"

class GeneralInfoElement : public LVObject, private LoopListener {
public:
	GeneralInfoElement(lv_obj_t* parent, DriveMode mode = DriveMode::Idle);
	~GeneralInfoElement();
	void setMode(DriveMode mode);
	void setMobBat(uint8_t index); //0-7

private:
	void setSignal(uint8_t index); //0-3
	void setConBat(uint8_t index); //0-7
	void loop(uint micros) override;

	lv_obj_t* modeImg;
	SignalElement* signal;
	BatteryElement* conBat;
	BatteryElement* mobBat;

	static const char* modePaths[5];

	uint32_t counter = 0;
	static constexpr uint32_t second = 1000000;
	uint8_t conBatIndex;
};


#endif //BATCONTROLLER_FIRMWARE_GENERALINFOELEMENT_H
