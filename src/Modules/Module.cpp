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
	checkCounter++;
	if(connected && checkCounter >= ReadInterval){
		checkCounter = 0;
		transmission(micros);
	}else if(checkCounter >= CheckInterval){
		checkCounter = 0;
		checkConnection();
		if(isConnected()){
			init();
		}
	}
}

}

void Module::errorOccured(){
	connected = false;
	checkCounter = 0;
}




