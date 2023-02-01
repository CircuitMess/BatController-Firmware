#include "AutoControls.h"
#include <BatController.h>
#include <Com/Communication.h>

AutoControls::AutoControls(lv_obj_t* container) : dirElement(container){
	lv_obj_update_layout(container);
	auto dirElementX = lv_obj_get_width(container) - lv_obj_get_width(dirElement.getLvObj()) - 3;
	auto dirElementY = lv_obj_get_height(container) - lv_obj_get_height(dirElement.getLvObj()) - 3;
	lv_obj_set_pos(dirElement.getLvObj(), dirElementX, dirElementY);
	setSpeed(0);
}

void AutoControls::start(){
	BatController.getInput()->addListener(this);
}

void AutoControls::stop(){
	BatController.getInput()->removeListener(this);
}

void AutoControls::buttonPressed(uint i){
	if(i == BTN_B){
		Com.sendHonk();
	}
}

void AutoControls::setSpeed(uint8_t speed){
	dirElement.setSpeed(speed);

	if(speed == 0){
		lv_obj_add_flag(dirElement.getLvObj(), LV_OBJ_FLAG_HIDDEN);
	}else{
		lv_obj_clear_flag(dirElement.getLvObj(), LV_OBJ_FLAG_HIDDEN);
	}
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

	dirElement.setDirection(angle);

	const uint8_t speed = (abs(motors.frontRight) + abs(motors.frontLeft) + abs(motors.backRight) + abs(motors.backLeft)) / 4;
	setSpeed(speed);
}
