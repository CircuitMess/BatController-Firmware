#include "ManualDriver.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <BatController.h>
#include <Com/Communication.h>

static const uint directions[] = { BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT };

ManualDriver::ManualDriver(lv_obj_t* elementContainer) : boost(elementContainer){
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
	if(i == BTN_A){
		if(boostGauge > 0) boostActive = true;
		Com.sendBoost(boostActive);
		LoopManager::addListener(this);
	}else if(i == BTN_B){
		Com.sendHonk();
	}else{
		switch(i){
			case BTN_UP:
				dir |= 0b0001;
				break;
			case BTN_DOWN:
				dir |= 0b0010;
				break;
			case BTN_LEFT:
				dir |= 0b0100;
				break;
			case BTN_RIGHT:
				dir |= 0b1000;
				break;
			default:
				break;
		}
		Com.sendDriveDir(dir);
	}
}

void ManualDriver::buttonReleased(uint i){
	if(i == BTN_A){
		boostActive = false;
		Com.sendBoost(boostActive);
	}else{
		switch(i){
			case BTN_UP:
				dir &= ~0b0001;
				break;
			case BTN_DOWN:
				dir &= ~0b0010;
				break;
			case BTN_LEFT:
				dir &= ~0b0100;
				break;
			case BTN_RIGHT:
				dir &= ~0b1000;
				break;
			default:
				break;
		}
		Com.sendDriveDir(dir);
	}
}

void ManualDriver::loop(uint micros){
	if(!boostActive && boostGauge == 100 || boostActive && boostGauge == 0) return;

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
