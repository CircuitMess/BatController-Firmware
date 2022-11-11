#include "AutoControls.h"
#include <BatController.h>
#include <Com/Communication.h>

AutoControls::AutoControls(lv_obj_t* container, LVScreen* parentScreen) : dirElement(container), speedModal(parentScreen, [](uint8_t speed){
	Com.sendDriveSpeed(speed);
}, 0){
	lv_obj_update_layout(container);
	auto dirElementX = lv_obj_get_width(container) - lv_obj_get_width(dirElement.getLvObj()) - 3;
	auto dirElementY = lv_obj_get_height(container) - lv_obj_get_height(dirElement.getLvObj()) - 3;
	lv_obj_set_pos(dirElement.getLvObj(), dirElementX, dirElementY);
}

void AutoControls::start(){
	BatController.getInput()->addListener(this);
	Com.sendDriveSpeed(0);
}

void AutoControls::stop(){
	BatController.getInput()->removeListener(this);
	if(speedModal.isActive()) speedModal.stop();
}

void AutoControls::buttonPressed(uint i){
	if(i == BTN_B){
		Com.sendHonk();
	}else if(i == BTN_UP || i == BTN_DOWN){
		if(!speedModal.isActive()) speedModal.start();
	}
}

void AutoControls::setSpeed(uint8_t speed){
	dirElement.setSpeed(speed);
}

void AutoControls::setDirection(float angle){
	dirElement.setDirection(angle);
}


