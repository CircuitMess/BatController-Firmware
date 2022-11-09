#ifndef BATCONTROLLER_FIRMWARE_MARKERACTIONELEMENT_H
#define BATCONTROLLER_FIRMWARE_MARKERACTIONELEMENT_H


#include "../Interface/LVObject.h"
#include <DriveInfo.h>

class MarkerActionElement : public LVObject {
public:
	explicit MarkerActionElement(lv_obj_t* parent, MarkerAction action);
	void setAction(MarkerAction action);

private:
	lv_obj_t* img;
	static constexpr uint8_t x = 136;
	static constexpr uint8_t y = 14;
};


#endif //BATCONTROLLER_FIRMWARE_MARKERACTIONELEMENT_H
