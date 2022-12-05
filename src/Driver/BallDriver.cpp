#include "BallDriver.h"
#include <Com/Communication.h>
#include <Pins.hpp>
#include <Input/Input.h>

BallDriver::BallDriver(lv_obj_t* container, LVScreen* screen) : autoControls(container, screen), hueElement(container, 120),
																hueModal(screen, [this](uint8_t hue){
																	//TODO - update BallHue in settings
																	Com.sendBallHue(hue);
																	hueElement.setHue(hue);
																}, 120){
}

BallDriver::~BallDriver() {
    stop();
}

void BallDriver::onFrame(const DriveInfo& frame, Color* pixels){
	if(frame.mode != DriveMode::Ball) return;

	const float leftMotors = (frame.motors.frontLeft + frame.motors.backLeft) / 2;
	const float rightMotors = (frame.motors.frontRight + frame.motors.backRight) / 2;
	float angle = (leftMotors - rightMotors) / (abs(leftMotors) + abs(rightMotors)) * 90;
	if(leftMotors < 0 && rightMotors < 0) angle += 180;
	autoControls.setDirection(angle);
}

void BallDriver::buttonPressed(uint i){
	if((i == BTN_LEFT || i == BTN_RIGHT) && !hueModal.isActive()){
		hueModal.start();
	}
}

void BallDriver::onStart(){
	autoControls.start();
	Input::getInstance()->addListener(this);
}

void BallDriver::onStop(){
    Serial.println("ON STOP");
	autoControls.stop();
	Input::getInstance()->removeListener(this);
}

