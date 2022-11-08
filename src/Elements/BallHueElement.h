#ifndef BATCONTROLLER_FIRMWARE_BALLHUEELEMENT_H
#define BATCONTROLLER_FIRMWARE_BALLHUEELEMENT_H


#include "../Interface/LVObject.h"

class BallHueElement : public LVObject {
public:
	BallHueElement(lv_obj_t* parent, uint8_t hue);
	void setHue(uint8_t hue);

private:
	lv_obj_t* img;
	static constexpr uint8_t x = 146;
	static constexpr uint8_t y = 15;
	static constexpr lv_opa_t colorOpacity = LV_OPA_60;
};


#endif //BATCONTROLLER_FIRMWARE_BALLHUEELEMENT_H
