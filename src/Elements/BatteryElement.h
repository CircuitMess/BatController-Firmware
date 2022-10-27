#ifndef BATCONTROLLER_FIRMWARE_BATTERYELEMENT_H
#define BATCONTROLLER_FIRMWARE_BATTERYELEMENT_H

#include "../Interface/LVObject.h"

enum class BatType {
	Controller, Batmobile
};

class BatteryElement : public LVObject {
public:
	BatteryElement(lv_obj_t* parent, BatType type);
	void setLevel(uint8_t index);

private:
	lv_obj_t* img;
	lv_obj_t* device;
};


#endif //BATCONTROLLER_FIRMWARE_BATTERYELEMENT_H
