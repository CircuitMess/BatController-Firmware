#include "MarkerDriver.h"

MarkerDriver::MarkerDriver(lv_obj_t* container, LVScreen* screen) : autoControls(container, screen),
																	markerElement(container, MarkerAction::None){
}

MarkerDriver::~MarkerDriver(){
	stop();
}

void MarkerDriver::onFrame(const DriveInfo& frame, Color* pixels){
	auto markerInfo = frame.toMarker();
	if(markerInfo == nullptr) return;

	auto action = markerInfo->action;

	markerElement.setAction(action);

	const float leftMotors = (frame.motors.frontLeft + frame.motors.backLeft) / 2;
	const float rightMotors = (frame.motors.frontRight + frame.motors.backRight) / 2;
	float angle = 0;
	if(leftMotors != 0 && rightMotors != 0){
		angle = (leftMotors - rightMotors) / (abs(leftMotors) + abs(rightMotors)) * 90;
	}
	if(leftMotors < 0 && rightMotors < 0) angle += 180;
	autoControls.setDirection(angle);
}

void MarkerDriver::onStart(){
	autoControls.start();
}

void MarkerDriver::onStop(){
	autoControls.stop();
}
