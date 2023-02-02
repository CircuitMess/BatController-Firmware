#include <Loop/LoopManager.h>
#include "LowBatteryService.h"
#include <BatteryService.h>
#include <BatController.h>
#include <Com/Communication.h>

LowBatteryService BatteryShutdown;

void LowBatteryService::begin(){
	LoopManager::addListener(this);
	Com.addListener(this);
	checkCtrlBattery();
}

void LowBatteryService::end(){
	LoopManager::removeListener(this);
	Com.removeListener(this);
}

void LowBatteryService::loop(uint micros) {
	checkTimer += micros;

    if(modalShowing){
        shutdownTimer += micros;
        if(shutdownTimer >= ShutdownDelay){
			// TODO: optionally do something different based on shutdownType (ex. go to pair)

			if(Com.isConnected()){
				Com.sendShutdown([](bool acked){
					BatController.shutdown();
				});

				end();
			}else{
				BatController.shutdown();
			}

			return;
        }
    }else if(checkTimer >= CheckInterval){
		checkTimer = 0;
		checkCtrlBattery();
    }
}

void LowBatteryService::onBattery(uint8_t percent, bool charging) {
    if(percent <= 1 && !charging && !modalShowing){
		showModal(BatType::Batmobile);
    }
}

void LowBatteryService::checkCtrlBattery(){
	if(Battery.getPercentage() == 0 && !Battery.charging()){
		showModal(BatType::Controller);
	}
}

void LowBatteryService::showModal(BatType type){
	if(modal){
		modal->stop();
		delete modal;
	}

	modalShowing = true;
	shutdownType = type;
	shutdownTimer = 0;

	modal = new LowBatteryModal(LVScreen::getCurrent(), type);
	modal->start();
}

void LowBatteryService::stopModal(){
	modalShowing = false;
	if(!modal) return;

	modal->stop();
	delete modal;
}
