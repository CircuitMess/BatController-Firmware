#include "BallDriver.h"
#include <Com/Communication.h>
#include <Pins.hpp>
#include <Input/Input.h>

BallDriver::BallDriver(lv_obj_t* container, LVScreen* screen) : autoControls(container, screen), hueElement(container, 120),
																hueModal(screen, [this](uint8_t hue){
																	Com.sendBallHue(hue);
																	hueElement.setHue(hue);
																}, 120){
}

BallDriver::~BallDriver() {
    stop();
}

void BallDriver::onFrame(const DriveInfo& frame, Color* pixels){
	if(frame.mode != DriveMode::Ball) return;

	autoControls.setDirection(frame.motors);
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

