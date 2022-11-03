#include "ManualDriver.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <BatController.h>
#include <Com/Communication.h>

static const uint directions[] = {BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT};

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
	if(i == BTN_LEFT || i == BTN_RIGHT || i == BTN_UP || i == BTN_DOWN){
		uint8_t dir = 0;
		for(int j = 0; j < 4; ++j){
			dir |= (Input::getInstance()->getButtonState(directions[j]) << j);
		}
		Com.sendDriveDir(dir);
	}else if(i == BTN_A){
		if(boostGauge > 0) boostActive = true;
		Com.sendBoost(boostActive);
	}else if(i == BTN_B){
		Com.sendHonk();
	}
}

void ManualDriver::buttonReleased(uint i){
	if(i == BTN_LEFT || i == BTN_RIGHT || i == BTN_UP || i == BTN_DOWN){
		uint8_t dir = 0;
		for(int j = 0; j < 4; ++j){
			dir |= (Input::getInstance()->getButtonState(directions[j]) << j);
		}
		Com.sendDriveDir(dir);
	}else if(i == BTN_A){
		boostActive = false;
		Com.sendBoost(boostActive);
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
		Com.sendBoost(false);
	}

	boost.setActive(boostActive);
	boost.setLevel(boostGauge);
}
