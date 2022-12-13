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
	autoControls.setDirection(frame.motors);
}

void MarkerDriver::onStart(){
	autoControls.start();
}

void MarkerDriver::onStop(){
	autoControls.stop();
}
