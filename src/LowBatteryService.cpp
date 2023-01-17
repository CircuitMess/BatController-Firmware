#include <Loop/LoopManager.h>
#include "LowBatteryService.h"
#include "Screens/PairScreen.h"
#include <BatteryService.h>
#include <BatController.h>
#include <Com/Communication.h>

LowBatteryService::LowBatteryService() {

}

LowBatteryService::~LowBatteryService() {
    stop();
}
void LowBatteryService::loop(uint micros) {
    checkTimer += micros;

    if(modalShowing){
        shutdownTimer += micros;
        if(shutdownTimer >= shutdownDelay){
            batModal->stop();
            shutdownTimer = 0;
            modalShowing = false;

            if(shutdownBattery == BatType::Controller){
                BatController.shutdown();
            }else if(shutdownBattery == BatType::Batmobile){
                //TODO: Send shutdown packet to batmobile
                //Com.sendShutdown([](bool temp){});
                auto pair = new PairScreen();
                pair->start();
            }
        }
    }else if(checkTimer >= checkInterval){
        checkTimer = 0;
        checkBattery();
    }
}

void LowBatteryService::onBattery(uint8_t percent, bool charging) {
    if(percent <= 2 && !charging){
        delete batModal;
        batModal = new LowBatteryModal(LVScreen().getCurrent(), BatType::Batmobile);
        batModal->start();
        modalShowing = true;
        shutdownBattery = BatType::Batmobile;
    }
}

void LowBatteryService::begin() {
    LoopManager::addListener(this);
    Com.addListener(this);
    checkBattery();
}

void LowBatteryService::stop() {
    LoopManager::removeListener(this);
    Com.removeListener(this);
}

void LowBatteryService::checkBattery() {
    if(isBatteryLow()){
        delete batModal;
        batModal = new LowBatteryModal(LVScreen().getCurrent(), BatType::Controller);
        batModal->start();
        modalShowing = true;
        shutdownBattery = BatType::Controller;
    }
}

bool LowBatteryService::isBatteryLow() {
    return (Battery.getPercentage() <= 2 && !Battery.charging());
}
