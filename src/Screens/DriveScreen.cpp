#include "DriveScreen.h"
#include <Input/Input.h>
#include <BatController.h>
#include <DriveMode.h>
#include "../Driver/ManualDriver.h"
#include "../Driver/BallDriver.h"
#include "../Driver/MarkerDriver.h"
#include "PairScreen.h"
#include "MainMenu.h"
#include "../Driver/SimpleProgDriver.h"
#include "SimpleProg/SimpleProgScreen.h"
#include <Com/Communication.h>

DriveScreen::DriveScreen(DriveMode mode, std::unique_ptr<Driver> customDriver) : LVScreen(), infoElement(obj, mode){
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
	if(customDriver){
		currentMode = mode;
		driver = std::move(customDriver);
		if(mode == DriveMode::SimpleProgramming){
			static_cast<SimpleProgDriver&>(*driver).setContainer(driverLayer);
		}
		Com.sendDriveMode(currentMode);
	}else{
		setMode(mode);
	}

	lv_group_add_obj(inputGroup, obj);
	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		auto &driveScreen = *(DriveScreen*)e->user_data;
		if(lv_event_get_key(e) != LV_KEY_HOME) return;

		bool backToMenu = driveScreen.currentMode != DriveMode::SimpleProgramming;

		driveScreen.stop();
		delete &driveScreen;

		if(backToMenu){
			auto mainMenu = new MainMenu();
			mainMenu->start();
		}else{
			auto simple = new SimpleProgScreen();
			simple->start();
		}

	}, LV_EVENT_KEY, this);
}

DriveScreen::~DriveScreen(){
	free(imgBuf);
	setMode(DriveMode::Idle);
}

void DriveScreen::onStarting(){
	memset(imgBuf, 0, 160 * 120 * 2);
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

}

void DriveScreen::onDisconnected(){
	stop();
	delete this;

	auto pair = new PairScreen();
	pair->start();
}
