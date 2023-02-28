#include "ManualDriver.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <BatController.h>
#include <Com/Communication.h>
#include <Wire.h>

ManualDriver::ManualDriver(lv_obj_t* elementContainer) : Driver(DriveMode::Manual), boost(new BoostElement(elementContainer)){
	lv_obj_set_pos(boost->getLvObj(), 2, 10);
	boost->setLevel(boostGauge);
}

ManualDriver::~ManualDriver(){
	stop();
	delete boost;
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
		boostPressed = true;
		if(dir != 0b0000){
			if(boostGauge > 0) boostActive = true;
			Com.sendBoost(boostActive);
			boostTimer = 0;
			boostGaugeStart = boostGauge;
		}
	}else if(i == BTN_B){
		Com.sendHonk();
	}else if(i == BTN_MENU){
		return;
	}else{
		if(boostPressed){
			if(boostGauge > 0) boostActive = true;
			Com.sendBoost(boostActive);
			boostTimer = 0;
			boostGaugeStart = boostGauge;
		}
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
		directionSendTimer = 0;
	}
}

void ManualDriver::buttonReleased(uint i){
	if(i == BTN_A){
		boostPressed = false;
		if(boostActive){
			boostActive = false;
			Com.sendBoost(boostActive);
			boostTimer = 0;
			boostGaugeStart = boostGauge;
		}
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
		directionSendTimer = 0;
	}

	if(dir == 0b0000 && boostActive){
		boostActive = false;
		Com.sendBoost(boostActive);
		boostTimer = 0;
		boostGaugeStart = boostGauge;
	}
}

void ManualDriver::sendDriveDir() const{
	auto direction = dir;
	if(gyro){
		direction |= gyroDir;
	}

	Serial.printf("%lu sendDriveDir\n", millis());
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
	directionSendTimer = 0;
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

	//no need to repeat setting motors to zero again and again
	if(!(lastDir == dir && dir == 0)){
		directionSendTimer += micros;
		if(directionSendTimer >= directionSendInterval){
			directionSendTimer = 0;
			sendDriveDir();
			lastDir = dir;
		}
	}

	if((!boostActive && boostGauge == 100) || (boostActive && boostGauge == 0)) return;

	boostTimer += micros;

	if(boostActive){
		boostGauge = max(boostGaugeStart - map(boostTimer, 0, boostDuration, 0, 100), 0L);
	}else{
		boostGauge = min(boostGaugeStart + map(boostTimer, 0, boostFillDuration, 0, 100), 100L);
	}

	if(boostActive && boostGauge == 0){
		boostActive = false;
		boostTimer = 0;
		boostGaugeStart = boostGauge;
		Com.sendBoost(false);
	}

	boost->setActive(boostActive);
	boost->setLevel(boostGauge);
}
