#include "DriveScreen.h"
#include <Input/Input.h>
#include <BatController.h>
#include <DriveMode.h>
#include "../Driver/ManualDriver.h"

DriveScreen::DriveScreen(DriveMode mode) : infoElement(obj, mode){
	topLayer = lv_obj_create(obj);
	lv_obj_set_size(topLayer, lv_obj_get_width(obj), lv_obj_get_height(obj));
	lv_obj_move_foreground(topLayer);

	canvas = lv_canvas_create(obj);
	lv_canvas_set_buffer(canvas, canvasBuf.data(), 160, 120, LV_IMG_CF_RAW);
	lv_obj_set_pos(canvas, 0, 4);
	lv_obj_move_background(canvas);


	switch(mode){
		case DriveMode::Idle:
			break;
		case DriveMode::Manual:
			driver = std::make_unique<ManualDriver>(feed, topLayer);
			break;
		case DriveMode::Ball:
			break;
		case DriveMode::Line:
			break;
		case DriveMode::Marker:
			break;
	}

	//TODO - feed.onFrame( draw na canvas)

	// If mode is idle, do nothing (setMode returns early)
	setMode(mode);
}

void DriveScreen::onStart(){
	driver->start();
	Input::getInstance()->addListener(this);
	Com.addDcListener(this);
}

void DriveScreen::onStop(){
	driver->stop();
	Input::getInstance()->removeListener(this);
	Com.removeDcListener(this);
}

void DriveScreen::setMode(DriveMode newMode){
	if(newMode == currentMode) return;

	driver.reset();

	static const std::function<std::unique_ptr<Driver>(lv_obj_t* elementContainer)> starter[5] = {
			[](lv_obj_t* elementContainer){ return nullptr; },
			[](lv_obj_t* elementContainer){ return std::make_unique<ManualDriver>(elementContainer); },
			[](lv_obj_t* elementContainer){ return nullptr; },
			[](lv_obj_t* elementContainer){ return nullptr; },
			[](lv_obj_t* elementContainer){ return nullptr; },
	};

	driver = starter[(int) newMode](nullptr);
	if(driver == nullptr){
		currentMode = DriveMode::Idle;
		return;
	}

	currentMode = newMode;
	Com.sendDriveMode(currentMode);
}

void DriveScreen::buttonPressed(uint i){
	switch(i){
		case BTN_B:
			//TODO - S3.honk, S3.blinkHeadlights, manual control override
			break;
		case BTN_MENU:
			pop();
			break;
	}
}

void DriveScreen::buttonReleased(uint i){

}

void DriveScreen::loop(uint micros){

}

void DriveScreen::onDisconnected(){
	stop();
	delete this;

	auto pair = new PairScreen();
	pair->start();
}
