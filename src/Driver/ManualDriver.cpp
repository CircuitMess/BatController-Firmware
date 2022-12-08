#include "ManualDriver.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <BatController.h>
#include <Com/Communication.h>
#include <Wire.h>

static const uint directions[] = { BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT };

ManualDriver::ManualDriver(lv_obj_t* elementContainer) : boost(elementContainer){
	lv_obj_set_pos(boost.getLvObj(), 2, 10);
}

ManualDriver::~ManualDriver() {
    stop();
}

void ManualDriver::onStart(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);

	checkGyro();
}

void ManualDriver::onStop(){
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
}

void ManualDriver::buttonPressed(uint i){
	if(i == BTN_A){
		if(boostGauge > 0) boostActive = true;
		Com.sendBoost(boostActive);
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

		sendDriveDir();
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

		sendDriveDir();
	}
}

void ManualDriver::sendDriveDir(){
	auto direction = dir;
	if(gyro){
		direction |= gyroDir;
	}

	Com.sendDriveDir(direction);
}

void ManualDriver::checkGyro(){
	gyroDir = 0;

	Wire.beginTransmission(GyroAddr);
	gyro = Wire.endTransmission() == 0;
	if(!gyro) return;

	uint8_t data[2] = { 0x20, 0b01010001 };

	Wire.beginTransmission(GyroAddr);
	Wire.write(data, sizeof(data));
	Wire.endTransmission();
}

void ManualDriver::sendGyro(){
	Wire.beginTransmission(GyroAddr);
	Wire.write(0x28);
	if(Wire.endTransmission() != 0){
		gyro = false;
		gyroDir = 0;
		return;
	}

	uint8_t data[6];
	Wire.requestFrom(GyroAddr, (uint8_t) 6);
	Wire.readBytes(data, 6);

	glm::vec<3, int16_t> accel = {
			(data[1] << 8) | data[0],
			(data[3] << 8) | data[2],
			(data[5] << 8) | data[4],
	};

	float x = (float) accel.x / 10000.0f;
	float y = (float) accel.y / 10000.0f;

	gyroDir = 0;

	if(y < -0.6) gyroDir |= 0b1000;
	else if(y > 0.6) gyroDir |= 0b0100;

	if(x < -0.6) gyroDir |= 0b0010;
	else if(x > 0.6) gyroDir |= 0b0001;

	sendDriveDir();
}

void ManualDriver::loop(uint micros){
	gyroCounter += micros;
	if(gyro && gyroCounter >= GyroReadInterval){
		gyroCounter = 0;
		sendGyro();
	}else if(!gyro && gyroCounter >= GyroCheckInterval){
		gyroCounter = 0;
		checkGyro();
	}

	directionSendTimer += micros;
	if(directionSendTimer >= directionSendInterval){
		directionSendTimer = 0;
		sendDriveDir();
	}


	if((!boostActive && boostGauge == 100) || (boostActive && boostGauge == 0)) return;

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
