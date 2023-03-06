#include "Module.h"
#include <Wire.h>
#include <Loop/LoopManager.h>

Module::Module(const uint8_t& addr) : address(addr){
}

Module::~Module(){
	end();
}

void Module::begin(){
	if(isConnected()){
		init();
		transmissionCounter = 0;
	}
	LoopManager::addListener(this);
}

void Module::end(){
	LoopManager::removeListener(this);
}

bool Module::isConnected() const{
	return connected;
}

void Module::checkConnection(){
	Wire.beginTransmission(address);
	connected = Wire.endTransmission() == 0;
}

void Module::loop(uint micros){
	checkCounter += micros;
	transmissionCounter += micros;
	if(connected && transmissionCounter >= ReadInterval){
		transmissionCounter = 0;
		transmission(micros);
	}else if(checkCounter >= CheckInterval){
		checkCounter = 0;
		bool prevConnected = connected;
		checkConnection();
		if(isConnected() && !prevConnected){
			init();
		}
	}
}


void Module::errorOccured(){
	connected = false;
	checkCounter = 0;
	transmissionCounter = 0;
}




