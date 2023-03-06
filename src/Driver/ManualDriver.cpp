#include "ManualDriver.h"
#include <Loop/LoopManager.h>
#include <Input/Input.h>
#include <BatController.h>
#include <Com/Communication.h>
#include <Wire.h>
#include "../Modules/AcceleroModule.h"
#include "../Modules/VibroModule.h"

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

	if(dir == 0b0000 && getGyroDir() == 0 && boostActive){
		boostActive = false;
		Com.sendBoost(boostActive);
		boostTimer = 0;
		boostGaugeStart = boostGauge;
	}
}

void ManualDriver::sendDriveDir(){
	//gyro value is pushed only when no button inputs, gyro detected and moved beyond deadzone
	if(!dir && accelero.isConnected() && getGyroSpeed()){
		lastDir = getGyroDir();
		Com.sendDriveDir(getGyroDir(), getGyroSpeed());
	}else{
		lastDir = dir;
		Com.sendDriveDir(dir);
	}
}

void ManualDriver::sendGyro(){
	if(boostPressed && getGyroDir() && !boostActive && boostGauge > 0){
		boostActive = true;
		Com.sendBoost(boostActive);
		boostTimer = 0;
		boostGaugeStart = boostGauge;
	}

	if(dir == 0 && getGyroDir() == 0 && boostActive){
		boostActive = false;
		Com.sendBoost(boostActive);
		boostTimer = 0;
		boostGaugeStart = boostGauge;
	}

	sendDriveDir();
	directionSendTimer = 0;
}

void ManualDriver::loop(uint micros){
	if(accelero.isConnected()){
		sendGyro();
	}

	//no need to repeat setting motors to zero again and again
	if(!(lastDir == dir && dir == 0)){
		directionSendTimer += micros;
		if(directionSendTimer >= directionSendInterval){
			directionSendTimer = 0;
			sendDriveDir();
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

uint8_t ManualDriver::getGyroDir() const{
	if(!accelero.isConnected()) return 0;

	auto& accel = accelero.getAccelerometer();
	uint8_t gyroDir = 0;

	if(accel.z > 0){
		const float y = constrain((float) accel.x / GyroRange, -1.0, 1.0);
		const float x = constrain((float) -accel.y / GyroRange, -1.0, 1.0);

		if(y < -GyroDeadzone) gyroDir |= 0b1000;
		else if(y > GyroDeadzone) gyroDir |= 0b0100;

		if(x < -GyroDeadzone) gyroDir |= 0b0010;
		else if(x > GyroDeadzone) gyroDir |= 0b0001;
	}

	return gyroDir;
}

uint8_t ManualDriver::getGyroSpeed() const{
	auto& accel = accelero.getAccelerometer();

	uint8_t gyroSpeed = 0;

	if(accel.z > 0){
		const float y = constrain((float) accel.x / GyroRange, -1.0, 1.0);
		const float x = constrain((float) -accel.y / GyroRange, -1.0, 1.0);
		const float speedVec = sqrt(pow(x, 2) + pow(y, 2));
		gyroSpeed = (constrain(speedVec - GyroDeadzone, 0, 1.0 - GyroDeadzone)) / (1.0 - GyroDeadzone) * GyroSpeedRange;
	}
	return gyroSpeed;
}
