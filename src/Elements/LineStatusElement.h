#ifndef BATCONTROLLER_FIRMWARE_LINESTATUSELEMENT_H
#define BATCONTROLLER_FIRMWARE_LINESTATUSELEMENT_H


#include <LVObject.h>
#include <DriveInfo.h>

class LineStatusElement : public LVObject {
public:
	explicit LineStatusElement(lv_obj_t* parent, LineStatus status);
	void setStatus(LineStatus status);

private:
	lv_obj_t* img;
	static constexpr uint8_t x = 144;
	static constexpr uint8_t y = 14;
};


#endif //BATCONTROLLER_FIRMWARE_LINESTATUSELEMENT_H
