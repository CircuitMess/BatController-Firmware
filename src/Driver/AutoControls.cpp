#include "AutoControls.h"
#include <BatController.h>
#include <Com/Communication.h>

AutoControls::AutoControls(lv_obj_t* container, LVScreen* parentScreen) : dirElement(container), speedModal(parentScreen, [this](uint8_t speed){
	Com.sendDriveSpeed(speed);
	dirElement.setSpeed(speed);
}, 0){
	lv_obj_update_layout(container);
	auto dirElementX = lv_obj_get_width(container) - lv_obj_get_width(dirElement.getLvObj()) - 3;
	auto dirElementY = lv_obj_get_height(container) - lv_obj_get_height(dirElement.getLvObj()) - 3;
	lv_obj_set_pos(dirElement.getLvObj(), dirElementX, dirElementY);
}

void AutoControls::start(){
	BatController.getInput()->addListener(this);
	Com.sendDriveSpeed(0);
}

void AutoControls::stop(){
	BatController.getInput()->removeListener(this);
	if(speedModal.isActive()) speedModal.stop();
}

void AutoControls::buttonPressed(uint i){
	if(i == BTN_B){
		Com.sendHonk();
	}else if(i == BTN_UP || i == BTN_DOWN){
		if(!speedModal.isActive()) speedModal.start();
	}
}

void AutoControls::setSpeed(uint8_t speed){
	dirElement.setSpeed(speed);
}

void AutoControls::setDirection(float angle){
	dirElement.setDirection(angle);
}

void AutoControls::setDirection(MotorInfo motors){
	const float leftMotors = (float) (motors.frontLeft + motors.backLeft) / 2.0f;
	const float rightMotors = (float) (motors.frontRight + motors.backRight) / 2.0f;

	float angle = 0;

	const float sum = abs(leftMotors) + abs(rightMotors);
	if(leftMotors != 0 && rightMotors != 0){
		angle = (leftMotors - rightMotors) / sum * 90;
	}

	if(leftMotors < 0 && rightMotors < 0) angle += 180;

	setDirection(angle);
}
