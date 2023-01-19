#include "ProgEditScreen.h"
#include <Pins.hpp>
#include <Input/Input.h>

ProgEditScreen::ProgEditScreen(Simple::Program& program, std::function<void()> saveCallback) : program(program), modal(this){
	//bildat UI

	lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_GREEN), LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_STATE_DEFAULT);
}

void ProgEditScreen::onStart(){
	Input::getInstance()->addListener(this);
}

void ProgEditScreen::onStop(){
	Input::getInstance()->removeListener(this);
	if(saveCallback) saveCallback();
}

void ProgEditScreen::buttonReleased(uint i){
	if((i == BTN_B || i == BTN_MENU) && !modal.isActive()){
		pop();
	}
}

