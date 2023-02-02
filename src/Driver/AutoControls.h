#ifndef BATCONTROLLER_FIRMWARE_AUTOCONTROLS_H
#define BATCONTROLLER_FIRMWARE_AUTOCONTROLS_H

#include <Loop/LoopListener.h>
#include <DriveInfo.h>
#include "../Elements/DriveDirectionElement.h"
#include "../Elements/SpeedModal.h"

class AutoControls : private InputListener {
public:
	AutoControls(lv_obj_t* container);
	void start();
	void stop();

	void setDirection(MotorInfo motors);

private:
	DriveDirectionElement dirElement;

	void buttonPressed(uint i) override;

	/**
	* @param speed [0-100]
	*/
	void setSpeed(uint8_t speed);
};


#endif //BATCONTROLLER_FIRMWARE_AUTOCONTROLS_H
