#include "InputLVGL.h"
#include <Input/Input.h>
#include <BatController.h>
#include <vector>
#include <lvgl.h>

const std::map<uint8_t, lv_key_t> InputLVGL::keyMap = {{ BTN_UP,    LV_KEY_PREV },
												 { BTN_DOWN,  LV_KEY_NEXT },
												 { BTN_LEFT,  LV_KEY_LEFT },
												 { BTN_RIGHT, LV_KEY_RIGHT },
												 { BTN_A,     LV_KEY_ENTER },
												 { BTN_B,     LV_KEY_ESC },
												 { BTN_MENU,  LV_KEY_ESC }};

InputLVGL* InputLVGL::instance = nullptr;
bool InputLVGL::verticalNavigation = true;

InputLVGL::InputLVGL(){
	instance = this;

	static lv_indev_drv_t inputDriver;
	lv_indev_drv_init(&inputDriver);
	inputDriver.type = LV_INDEV_TYPE_KEYPAD;
	inputDriver.long_press_repeat_time = 20;
	inputDriver.long_press_time = 350;
	inputDriver.read_cb = [](lv_indev_drv_t* drv, lv_indev_data_t* data){ InputLVGL::getInstance()->read(drv, data); };
	inputDevice = lv_indev_drv_register(&inputDriver);
}

void InputLVGL::read(lv_indev_drv_t* drv, lv_indev_data_t* data){
	if(lastKey == (uint32_t) -1) return;

	data->key = keyMap.at(lastKey);
	data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
	if(data->key == LV_KEY_PREV && !verticalNavigation) data->key = LV_KEY_UP;
	if(data->key == LV_KEY_NEXT && !verticalNavigation) data->key = LV_KEY_DOWN;
}

void InputLVGL::buttonReleased(uint i){
	if(!keyMap.count(i)) return;

	pressed = false;
	lastKey = i;
}

void InputLVGL::buttonPressed(uint i){
	if(!keyMap.count(i)) return;

	pressed = true;
	lastKey = i;
}

InputLVGL* InputLVGL::getInstance(){
	return instance;
}

lv_indev_t* InputLVGL::getIndev(){
	return inputDevice;
}

void InputLVGL::enableVerticalNavigation(bool enable){
	verticalNavigation = enable;
}



