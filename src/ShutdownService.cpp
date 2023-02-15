#include <Com/Communication.h>
#include <BatController.h>
#include <Loop/LoopManager.h>
#include "ShutdownService.h"
#include "Elements/MessageModal.h"

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

	if(timer >= ShutdownSeconds[Settings.get().shutdownTime] * 1000000 && !done){
		done = true;

		if(Com.isConnected()){
			LoopManager::removeListener(this);
			Com.sendShutdown([this](bool ackReceived){
				shutdown();
			});
		}else{
			LoopManager::removeListener(this);
			shutdown();
		}
	}
}

void ShutdownService::shutdown(){
	auto screen = LVScreen::getCurrent();
	screen->stop();

	auto modal = new MessageModal(screen, "Inactive.\nShutting down.", 5000);
	modal->setDismissCallback([](){
		BatController.shutdown();
	});
	modal->start();
}
