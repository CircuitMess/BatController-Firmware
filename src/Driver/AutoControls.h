#ifndef BATCONTROLLER_FIRMWARE_AUTOCONTROLS_H
#define BATCONTROLLER_FIRMWARE_AUTOCONTROLS_H

#include <Loop/LoopListener.h>
#include <DriveInfo.h>
#include "../Elements/DriveDirectionElement.h"
#include "../Elements/SpeedModal.h"

class AutoControls : private InputListener {
public:
	AutoControls(lv_obj_t* container, LVScreen* parentScreen);
	void start();
	void stop();

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

	void setDirection(MotorInfo motors);


private:
	DriveDirectionElement dirElement;
	SpeedModal speedModal;

	void buttonPressed(uint i) override;
};


#endif //BATCONTROLLER_FIRMWARE_AUTOCONTROLS_H
