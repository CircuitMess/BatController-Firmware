#include "PairScreen.h"
#include <lvgl.h>
#include <BatController.h>
#include <Com/Communication.h>
#include <Settings.h>
#include "DriveScreen.h"
#include "MainMenu.h"
#include <string.h>
#include <Loop/LoopManager.h>

PairScreen::PairScreen() : LVScreen(), scanAruco(obj, inputGroup), connecting(obj), error(obj, inputGroup), scanQR(obj, inputGroup),
						   input(obj, inputGroup){
	lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
	lv_obj_set_size(obj, 160, 128);


	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_top(obj, 128, 0);
	lv_obj_scroll_to_y(obj, 0, LV_ANIM_OFF);

	randID = rand() % 256;

	input.setNetwork(Settings.get().ssid);
	input.setPassword(Settings.get().password);

	wifi.setDoneCallback([this](){
		LoopManager::removeListener(this);
		connecting.stop();
		scanQR.start(ssid, password, wifi.getIPAddress());
	});

	scanAruco.setCallback([this](){
		scanAruco.stop();
		pair.stop();
		input.start();
	});

	input.setCallbackDone([this](std::string ssidInput, std::string passInput){
		memset(Settings.get().ssid, 0, sizeof(Settings.get().ssid));
		memset(Settings.get().password, 0, sizeof(Settings.get().password));
		memcpy(Settings.get().ssid, ssidInput.c_str(), ssidInput.size());
		memcpy(Settings.get().password, passInput.c_str(), passInput.size());
		Settings.store();

		ssid = ssidInput;
		password = passInput;

		wifi.start();

		microCounter = 0;
		LoopManager::addListener(this);

		input.stop();
		connecting.start();
	});

	input.setCallbackBack([this](){
		input.stop();
		scanAruco.start(randID);
	});

	error.setCallback([this](){
		error.stop();
		scanAruco.start(randID);
	});

	scanQR.setCallback([this](){
		pair.start(randID);
		scanQR.stop();
		scanAruco.start(randID);
	});

	pair.setDoneCallback([this](){
		stop();
		delete this;

		Com.sendVolume(Settings.get().soundVolume);

		auto mainMenu = new MainMenu();
		mainMenu->start();
	});
}

PairScreen::~PairScreen(){
}

void PairScreen::onStart(){
	scanAruco.start(randID);
	pair.start(randID);
	lv_obj_scroll_to_y(obj, 128, LV_ANIM_ON);
}

void PairScreen::onStop(){
	pair.stop();
}

void PairScreen::loop(uint micros){
	microCounter += micros;
	if(microCounter >= timeout){
		LoopManager::removeListener(this);
		connecting.stop();
		wifi.stop();

		error.start("Couldn't connect to " + ssid + " press A to enter aruco scan screen");
		return;
	}
}
