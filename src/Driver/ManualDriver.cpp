#include "ManualDriver.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <BatController.h>

ManualDriver::ManualDriver(Feed& feed, lv_obj_t* elementContainer) : boost(elementContainer){
	lv_obj_set_pos(boost.getLvObj(), 2, 10);
}

void ManualDriver::onStart(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
}

void ManualDriver::onStop(){
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
}

void ManualDriver::buttonPressed(uint i){
	//TODO - staviti Com::sendDriveDir za directione i honk/flash za BTN_B
	switch(i){
		case BTN_LEFT:
			break;
		case BTN_RIGHT:
			break;
		case BTN_UP:
			break;
		case BTN_DOWN:
			break;
		case BTN_B:
			break;
		case BTN_A:
			if(boostGauge > 0) boostActive = true;
			break;
		default:
			break;
	}
}

void ManualDriver::buttonReleased(uint i){
	//TODO - staviti Com::sendDriveDir stop za directione i honk/flash za BTN_B
	switch(i){
		case BTN_LEFT:
			break;
		case BTN_RIGHT:
			break;
		case BTN_UP:
			break;
		case BTN_DOWN:
			break;
		case BTN_B:
			break;
		case BTN_A:
			boostActive = false;
			break;
		default:
			break;
	}
}

void ManualDriver::loop(uint micros){
	boostTimer += micros;

	if(boostActive && boostTimer > boostConsumptionRate){
		boostGauge = max(boostGauge - 1, 0);
		boostTimer = 0;
	}else if(!boostActive && boostTimer > boostFillRate){
		boostGauge = min(boostGauge + 1, 100);
		boostTimer = 0;
	}

	if(boostActive && boostGauge == 0){
		boostActive = false;
	}

	boost.setActive(boostActive);
	boost.setLevel(boostGauge);
}
