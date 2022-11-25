#include "DriveScreen.h"
#include <Input/Input.h>
#include <BatController.h>
#include <DriveMode.h>
#include "../Driver/ManualDriver.h"
#include "../Driver/BallDriver.h"
#include "../Driver/MarkerDriver.h"
#include "PairScreen.h"
#include <Com/Communication.h>
#include <Loop/LoopManager.h>

DriveScreen::DriveScreen(DriveMode mode) : LVScreen(), infoElement(obj, mode), overrideElement(obj){
	lv_obj_add_flag(infoElement.getLvObj(), LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(infoElement.getLvObj(), 0, 0);

	img = lv_img_create(obj);
	lv_obj_set_size(img, 160, 120);
	lv_obj_add_flag(img, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_pos(img, 0, 8);

	imgBuf = static_cast<Color*>(heap_caps_malloc(160 * 120 * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_32BIT));
	imgDsc.data = (const uint8_t*) imgBuf;
	imgDsc.data_size = 160 * 120 * 2;
	imgDsc.header.w = 160;
	imgDsc.header.h = 120;
	imgDsc.header.cf = LV_IMG_CF_TRUE_COLOR;
	imgDsc.header.always_zero = 0;
	lv_img_set_src(img, &imgDsc);

	driverLayer = lv_obj_create(obj);
	lv_obj_add_flag(driverLayer, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_size(driverLayer, lv_obj_get_width(obj), lv_obj_get_height(obj));
	lv_obj_move_foreground(driverLayer);

	feed.onFrame([this](std::shared_ptr<const DriveInfo> info, const Color* frame){
		if(!isRunning()) return;

		memcpy(imgBuf, frame, 160 * 120 * 2);

		if(driver){
			driver->onFrame(*info, imgBuf);
		}

		lv_obj_invalidate(img);
	});

	// If mode is idle, do nothing (setMode returns early)
	setMode(mode);

	lv_obj_add_flag(overrideElement.getLvObj(), LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_align(overrideElement.getLvObj(), LV_ALIGN_CENTER, 0, 0);
	lv_obj_move_foreground(overrideElement.getLvObj());
	hideOverrideElement();
}

DriveScreen::~DriveScreen(){
	free(imgBuf);
}

void DriveScreen::onStarting(){
	memset(imgBuf, 0xff, 160 * 120 * 2);
}

void DriveScreen::onStart(){
	if(!driver) return;

	driver->start();
	Input::getInstance()->addListener(this);
	Com.addDcListener(this);
}

void DriveScreen::onStop(){
	if(!driver) return;

	driver->stop();
	Input::getInstance()->removeListener(this);
	Com.removeDcListener(this);
}

void DriveScreen::setMode(DriveMode newMode){
	if(newMode == currentMode) return;
	driver.reset();

	if(newMode == DriveMode::Idle){
		currentMode = newMode;
		Com.sendDriveMode(DriveMode::Idle);
		return;
	}

	static const std::map<DriveMode, std::function<std::unique_ptr<Driver>(lv_obj_t* elementContainer, LVScreen* screen)>> Starters = {
			{ DriveMode::Manual, [](lv_obj_t* elementContainer, LVScreen* screen){ return std::make_unique<ManualDriver>(elementContainer); }},
			{ DriveMode::Ball,   [](lv_obj_t* elementContainer, LVScreen* screen){ return std::make_unique<BallDriver>(elementContainer, screen); }},
			{ DriveMode::Marker, [](lv_obj_t* elementContainer, LVScreen* screen){ return std::make_unique<MarkerDriver>(elementContainer, screen); }},
			{ DriveMode::Line,   [](lv_obj_t* elementContainer, LVScreen* screen){ return nullptr; }}
	};

	auto starter = Starters.at(newMode);
	if(!starter || (driver = starter(driverLayer, this)) == nullptr){
		currentMode = DriveMode::Idle;
		Com.sendDriveMode(currentMode);
		return;
	}

	currentMode = newMode;
	Com.sendDriveMode(currentMode);
}

void DriveScreen::buttonPressed(uint i){
	switch(i){
		case BTN_A:
			Com.sendHonk();
			break;
		case BTN_MENU:
			pop();
			break;
		case BTN_B:
			if(currentMode == DriveMode::Manual && previousMode == DriveMode::Idle) return;
			LoopManager::addListener(this);
			break;
	}
}

void DriveScreen::buttonReleased(uint i){
	switch(i){
		case BTN_B:
			if(currentMode == DriveMode::Manual && previousMode == DriveMode::Idle) return;
            LoopManager::removeListener(this);
            hideOverrideElement();
			break;
	}
}

void DriveScreen::onDisconnected(){
	stop();
	delete this;

	auto pair = new PairScreen();
	pair->start();
}

void DriveScreen::loop(uint micros){
	overrideCounter += micros;
	if(elementHidden){
		showElementCounter += micros;
		if(showElementCounter >= ShowElementTime){
			showOverrideElement();
		}
	}
	if(overrideCounter >= OverrideTime){
		overrideCounter = 0;
		overridePercent += 5;
		overrideElement.fill(overridePercent);
		if(overridePercent >= 115){
			setMode(DriveMode::Manual);
			hideOverrideElement();
			LoopManager::removeListener(this);
			return;
		}
	}
}

void DriveScreen::showOverrideElement() {
    if(currentMode == DriveMode::Manual){
        overrideElement.setText(overrideText::Auto);
    }else{
        overrideElement.setText(overrideText::Manual);
    }
    lv_obj_clear_flag(overrideElement.getLvObj(), LV_OBJ_FLAG_HIDDEN);
    elementHidden = false;
}

void DriveScreen::hideOverrideElement(){
	overridePercent = 0;
	overrideElement.fill(overridePercent);
	lv_obj_add_flag(overrideElement.getLvObj(), LV_OBJ_FLAG_HIDDEN);
	elementHidden = true;
	showElementCounter = 0;
}
