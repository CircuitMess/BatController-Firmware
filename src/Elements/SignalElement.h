#ifndef BATCONTROLLER_FIRMWARE_SIGNALELEMENT_H
#define BATCONTROLLER_FIRMWARE_SIGNALELEMENT_H

#include "../Interface/LVObject.h"

class SignalElement : public LVObject {
public:
	SignalElement(lv_obj_t* parent);
	void setLevel(uint8_t index);
private:
	lv_obj_t* img;
};

#endif //BATCONTROLLER_FIRMWARE_SIGNALELEMENT_H