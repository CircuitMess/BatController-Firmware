#ifndef BATCONTROLLER_FIRMWARE_DRIVEDIRECTIONELEMENT_H
#define BATCONTROLLER_FIRMWARE_DRIVEDIRECTIONELEMENT_H

#include <LVObject.h>

class DriveDirectionElement : public LVObject {
public:
	DriveDirectionElement(lv_obj_t* parent);
	~DriveDirectionElement() override;
	/**
	 * @param speed [0-100]
	 */
	void setSpeed(uint8_t speed);
	/**
	 * @param angle [0-360]
	 * it could be any number, positive or negative
	 * or out of the range, the code will set it
	 * in range
	 */
	void setDirection(float angle);

private:
	void drawFill();
	void drawRotation();

	float angle = 0;
	uint8_t speed = 0;

	lv_obj_t* canvasMain;
	lv_obj_t* canvasHelper;

	lv_color_t* bufferMain;
	lv_color_t* bufferHelper;

	static constexpr const char* arrowEmptyPath = "/DriveDir/Arrow0.raw";
	static constexpr const char* arrowFullPath = "/DriveDir/Arrow1.raw";
};

#endif //BATCONTROLLER_FIRMWARE_DRIVEDIRECTIONELEMENT_H