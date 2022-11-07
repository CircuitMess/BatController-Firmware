#ifndef BATCONTROLLER_FIRMWARE_DRIVEDIRECTIONELEMENT_H
#define BATCONTROLLER_FIRMWARE_DRIVEDIRECTIONELEMENT_H

#include "../Interface/LVObject.h"

class DriveDirectionElement : public LVObject {
public:
	DriveDirectionElement(lv_obj_t* parent);
	/**
	 * @param speed [0-100]
	 */
	void setSpeed(uint8_t speed);
	void setDirection(float angle);

private:
	void drawFill();
	void drawRotation();

	float angle;
	uint8_t speed;

	lv_obj_t* canvasMain;
	lv_obj_t* canvasHelper;

	lv_color_t* bufferMain;
	lv_color_t* bufferHelper;

	static constexpr const char* arrowEmptyPath = "/DriveDir/Arrow0.raw";
	static constexpr const char* arrowFullPath = "/DriveDir/Arrow1.raw";
};

#endif //BATCONTROLLER_FIRMWARE_DRIVEDIRECTIONELEMENT_H