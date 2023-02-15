#include <Com/Communication.h>
#include <BatController.h>
#include <Loop/LoopManager.h>
#include "ShutdownService.h"

ShutdownService AutoShutdown;

void ShutdownService::begin(){
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);
}

void ShutdownService::pause(){
	paused = true;
}

void ShutdownService::resume(){
	paused = false;
	timer = 0;
}

void ShutdownService::anyKeyPressed(){
	timer = 0;
}

void ShutdownService::loop(uint micros){
	if(paused || Settings.get().shutdownTime == 0) return;
	timer += micros;

	if(timer >= ShutdownSeconds[Settings.get().shutdownTime] * 1000000){

		if(Com.isConnected()){
			LoopManager::removeListener(this);
			Com.sendShutdown([this](bool ackReceived){
				BatController.shutdown();
			});
		}else{
			LoopManager::removeListener(this);
			BatController.shutdown();
		}
	}
}
