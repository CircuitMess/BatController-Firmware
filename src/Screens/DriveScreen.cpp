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
#include "../Driver/DanceDriver.h"
#include <Com/Communication.h>
#include <Loop/LoopManager.h>

DriveScreen::DriveScreen(DriveMode mode, std::unique_ptr<Driver> customDriver) : LVScreen(), overrideElement(obj){
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

		if(driver && info->mode == driver->getMode()){
			driver->onFrame(*info, imgBuf);
		}

		lv_obj_invalidate(img);
	});

	if(mode != DriveMode::Manual && mode != DriveMode::Idle){
		originalMode = mode;
	}

	if(customDriver){
		currentMode = mode;
		driver = std::move(customDriver);
		if(mode == DriveMode::SimpleProgramming){
			static_cast<SimpleProgDriver*>(driver.get())->setContainer(driverLayer);
		}
		Com.sendDriveMode(currentMode);
	}else{
		// If mode is idle, do nothing (setMode returns early)
		setMode(mode);
	}

	lv_obj_add_flag(overrideElement.getLvObj(), LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_align(overrideElement.getLvObj(), LV_ALIGN_CENTER, 0, 0);
	lv_obj_move_foreground(overrideElement.getLvObj());
	hideOverrideElement();

	lv_group_add_obj(inputGroup, obj);
	lv_obj_add_event_cb(obj, [](lv_event_t* e){
		auto &driveScreen = *(DriveScreen*)e->user_data;
		if(lv_event_get_key(e) != LV_KEY_HOME) return;

		bool backToMenu = driveScreen.currentMode != DriveMode::SimpleProgramming;

		auto info = std::move(driveScreen.infoElement);
		auto tmpScr = lv_obj_create(nullptr);
		if(info){
			lv_obj_set_parent(info->getLvObj(), tmpScr);
		}

		driveScreen.stop();
		delete &driveScreen;

		if(backToMenu){
			auto mainMenu = new MainMenu();
			mainMenu->setInfoElement(std::move(info));
			lv_obj_del(tmpScr);
			mainMenu->start();
		}else{
			// TODO: general info element passing
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
	if(infoElement == nullptr){
		infoElement = std::make_unique<GeneralInfoElement>(getLvObj(), currentMode);
	}

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
	if(driver){
		driver->stop();
	}
	driver.reset();

	if(newMode == DriveMode::Idle){
		currentMode = newMode;
		Com.sendDriveMode(DriveMode::Idle);
		if(infoElement){
			infoElement->setMode(DriveMode::Idle);
		}
		return;
	}

	static const std::map<DriveMode, std::function<std::unique_ptr<Driver>(lv_obj_t* elementContainer, LVScreen* screen)>> Starters = {
			{ DriveMode::Manual, [](lv_obj_t* elementContainer, LVScreen* screen){ return std::make_unique<ManualDriver>(elementContainer); }},
			{ DriveMode::Ball,   [](lv_obj_t* elementContainer, LVScreen* screen){ return std::make_unique<BallDriver>(elementContainer, screen); }},
			{ DriveMode::Marker, [](lv_obj_t* elementContainer, LVScreen* screen){ return std::make_unique<MarkerDriver>(elementContainer); }},
			{ DriveMode::Dance,  [](lv_obj_t* elementContainer, LVScreen* screen){ return std::make_unique<DanceDriver>(elementContainer); }}
	};

	auto starter = Starters.at(newMode);
	if(!starter || (driver = starter(driverLayer, this)) == nullptr){
		currentMode = DriveMode::Idle;
		Com.sendDriveMode(currentMode);
		if(infoElement){
			infoElement->setMode(DriveMode::Idle);
		}
		return;
	}

	currentMode = newMode;
	Com.sendDriveMode(currentMode);
	if(infoElement){
		infoElement->setMode(currentMode);
	}
}

void DriveScreen::buttonPressed(uint i){
	if(i == BTN_B){
		if(currentMode == DriveMode::Manual && originalMode == DriveMode::Idle) return;
		LoopManager::addListener(this);
		overrideTime = millis();
		overrideDone = false;
	}
}

void DriveScreen::buttonReleased(uint i){
	if(i == BTN_B){
		if(currentMode == DriveMode::Manual && originalMode == DriveMode::Idle) return;
		LoopManager::removeListener(this);
		hideOverrideElement();
		overrideTime = 0;
		overrideDone = false;
	}
}

void DriveScreen::onDisconnected(){
	stop();
	delete this;

	auto pair = new PairScreen(true);
	pair->start();
}

void DriveScreen::setInfoElement(std::unique_ptr<GeneralInfoElement> infoElement) {
	if(infoElement == nullptr){
		this->infoElement.reset();
		return;
	}

    this->infoElement = std::move(infoElement);
    this->infoElement->setMode(currentMode);
	lv_obj_set_parent(this->infoElement->getLvObj(), getLvObj());
}

void DriveScreen::loop(uint micros){
	if(overrideTime == 0){
		LoopManager::removeListener(this);
		hideOverrideElement();
		return;
	}

	uint32_t t = millis();
	const uint8_t perc = constrain(100 * (t - (overrideTime + OverrideShowDuration)) / (OverrideDuration - OverrideShowDuration), 0, 100);

	if(t - overrideTime >= OverrideShowDuration){
		if(!overrideShown){
			showOverrideElement();
		}

		overrideElement.fill(perc);
	}

	if(t - overrideTime >= (OverrideDuration + 100) && perc >= 100){
		if(!overrideDone){
			overrideDone = true;
			return;
		}

		hideOverrideElement();
		overrideTime = 0;
		overrideDone = false;
		LoopManager::removeListener(this);

		if(currentMode == DriveMode::Manual){
			setMode(originalMode);
		}else{
			setMode(DriveMode::Manual);
		}

		if(driver){
			driver->start();
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
    overrideShown = true;
}

void DriveScreen::hideOverrideElement(){
	lv_obj_add_flag(overrideElement.getLvObj(), LV_OBJ_FLAG_HIDDEN);
	overrideShown = false;
}
