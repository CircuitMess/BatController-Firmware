#include "DanceDriver.h"
#include <Pins.hpp>
#include <Input/Input.h>
#include <Com/Communication.h>

DanceDriver::DanceDriver(lv_obj_t* elementContainer) : Driver(DriveMode::Dance), danceElement(elementContainer){
	danceElement.setCurrentDance((DanceType) danceIndex);
}

DanceDriver::~DanceDriver(){
	stop();
}

void DanceDriver::onStart(){
	Input::getInstance()->addListener(this);
}

void DanceDriver::onStop(){
	Input::getInstance()->removeListener(this);
}

void DanceDriver::buttonReleased(uint i){
	if(i == BTN_LEFT){
		danceElement.arrowLeftPressed(false);
	}
	if(i == BTN_RIGHT){
		danceElement.arrowRightPressed(false);
	}
}

void DanceDriver::buttonPressed(uint i){
	if(i == BTN_RIGHT){
		danceElement.arrowRightPressed(true);
		if(danceIndex == 5){
			danceIndex = 0;
		}else{
			danceIndex++;
		}
		danceElement.setCurrentDance((DanceType) danceIndex);
		Com.sendDance((DanceType) danceIndex);
	}
	if(i == BTN_LEFT){
		danceElement.arrowLeftPressed(true);
		if(danceIndex == 0){
			danceIndex = 5;
		}else{
			danceIndex--;
		}
		danceElement.setCurrentDance((DanceType) danceIndex);
		Com.sendDance((DanceType) danceIndex);
	}
}
