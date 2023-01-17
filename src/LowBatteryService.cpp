#include <Loop/LoopManager.h>
#include "LowBatteryService.h"
#include "Screens/PairScreen.h"
#include <BatteryService.h>
#include <BatController.h>
#include <Com/Communication.h>

LowBatteryService::LowBatteryService() {
    LoopManager::addListener(this);
    Com.addListener(this);
}

LowBatteryService::~LowBatteryService() {
    LoopManager::removeListener(this);
    Com.removeListener(this);
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
                //TODO: Send shutdown packet to batmobile
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
