#include "DanceDriver.h"
#include <Pins.hpp>
#include <Input/Input.h>
#include <Com/Communication.h>

DanceDriver::DanceDriver(lv_obj_t *elementContainer) : dance(new DanceElement(elementContainer)){
    dance->setCurrentDance((DanceType)danceIndex);
}

DanceDriver::~DanceDriver() {

}

void DanceDriver::onStart() {
    Input::getInstance()->addListener(this);
}

void DanceDriver::onStop() {
    Input::getInstance()->removeListener(this);
}

void DanceDriver::buttonReleased(uint i) {
    if(i == BTN_LEFT){
        dance->arrowLeftPressed(false);
    }
    if(i == BTN_RIGHT){
        dance->arrowRightPressed(false);
    }
}

void DanceDriver::buttonPressed(uint i) {
    if(i == BTN_RIGHT){
        dance->arrowRightPressed(true);
        if(danceIndex == 3){
            danceIndex = 0;
        }else{
            danceIndex++;
        }
        dance->setCurrentDance((DanceType)danceIndex);
        Com.sendDance((DanceType) danceIndex);
    }
    if(i == BTN_LEFT){
        dance->arrowLeftPressed(true);
        if(danceIndex == 0){
            danceIndex = 3;
        }else{
            danceIndex--;
        }
        dance->setCurrentDance((DanceType)danceIndex);
        Com.sendDance((DanceType) danceIndex);
    }
}
