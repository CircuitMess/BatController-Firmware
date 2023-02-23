#include "PairScreen.h"
#include <lvgl.h>
#include <BatController.h>
#include <Com/Communication.h>
#include <Settings.h>
#include "DriveScreen.h"
#include "MainMenu.h"
#include <string.h>
#include "../Elements/MessageModal.h"

PairScreen::PairScreen(bool disconnect) : LVScreen(), scanAruco(obj, inputGroup), connecting(obj), error(obj, inputGroup), scanQR(obj, inputGroup),
										  input(obj, inputGroup), disconnect(disconnect), scanning(obj, inputGroup){
	lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
	lv_obj_set_size(obj, 160, 128);

	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);

	resetDirect();

	scanAruco.setCallback([this](){
		scanAruco.stop();
		pair.stop();

		scanning.start();
	});

	input.setCallbackDone([this](std::string ssidInput, std::string passInput){
		if(ssidInput.size() > 24){
			ssidInput.resize(24);
		}

		if(passInput.size() > 23){
			passInput.resize(23);
		}

		ssid = std::move(ssidInput);
		password = std::move(passInput);

		memset(Settings.get().ssid, 0, sizeof(Settings.get().ssid));
		memset(Settings.get().password, 0, sizeof(Settings.get().password));
		memcpy(Settings.get().ssid, ssid.c_str(), ssid.size());
		memcpy(Settings.get().password, password.c_str(), password.size());
		Settings.store();


		pair.start(ssid.c_str(), password.c_str(), false);

		input.stop();
		connecting.start();
	});

	input.setCallbackBack([this](){
		input.stop();

		resetDirect();
		scanAruco.start(randID);

		pair.stop();
		pair.start(directSSID, directPass, true);
	});

	error.setCallback([this](){
		error.stop();

		resetDirect();
		scanAruco.start(randID);

		pair.stop();
		pair.start(directSSID, directPass, true);
	});

	scanQR.setCallback([this](){
		scanQR.stop();

		resetDirect();
		scanAruco.start(randID);

		pair.stop();
		pair.start(directSSID, directPass, true);
	});

	pair.setDoneCallback([this](PairError pairError){
		switch(pairError){
			case PairError::PairOk:{
				auto scr = lv_obj_create(nullptr);
				lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
				lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
				lv_scr_load(scr);
				lv_timer_handler();

				stop();
				delete this;

				BatController.setBrightness(Settings.get().screenBrightness);
				Com.sendVolume(Settings.get().soundVolume);

				auto mainMenu = new MainMenu();
				mainMenu->setInfoElement(std::make_unique<GeneralInfoElement>(mainMenu->getLvObj()));
				mainMenu->start();

				lv_obj_del(scr);
				break;
			}

			case PairError::ExternalWiFiTimeout:
				connecting.stop();
				scanQR.stop();
				error.start("Couldn't connect to network.\n\nPress any key.");
				break;


			case PairError::ServerError:
				connecting.stop();
				error.start("Couldn't start Com server.\n\nPress any key.");
				break;


			case PairError::ExternalWiFiConnected:
				connecting.stop();
				scanQR.start(ssid, password, WiFiService::getIPAddress());
				break;
		}
	});

	scanning.setCallbackError([this](std::string errorMessage){
		scanning.stop();
		error.start(errorMessage);
	});

	scanning.setCallbackBack([this](){
		scanning.stop();

		resetDirect();
		scanAruco.start(randID);

		pair.stop();
		pair.start(directSSID, directPass, true);
	});

	scanning.setCallbackDone([this](std::string ssid){
		scanning.stop();
		if(ssid.size() > 24){
			ssid.resize(24);
		}

		this->ssid = std::move(ssid);
		if(strcmp(this->ssid.c_str(), Settings.get().ssid) == 0){
			input.setPassword(Settings.get().password);
		}else{
			input.setPassword("");
		}

		input.start();
	});

}

PairScreen::~PairScreen(){
}

void PairScreen::onStarting(){
	resetDirect();
	scanAruco.start(randID);
}

void PairScreen::onStart(){
	pair.start(directSSID, directPass, true);
	if(disconnect){
		auto modal = new MessageModal(this, "Disconnected\nfrom\nBatmobile!", 5000);
		modal->start();
	}
}

void PairScreen::onStop(){
	pair.stop();
}

void PairScreen::resetDirect(){
	randID = rand() % 512;
	printf("Rand ID: %d\n", randID);

	memcpy(directSSID, "Batmobile ", 10);
	directSSID[10] = (randID / 100) + '0';
	directSSID[11] = ((randID / 10) % 10) + '0';
	directSSID[12] = (randID % 10) + '0';
	directSSID[13] = '\0';

	memset(directPass, 0, 10);
	const char* batmobile = "Batmobile";
	for(int i = 0; i < 9; i++){
		char temp = batmobile[i];
		temp = temp + randID * 5 + 16;
		temp = temp % ('z' - 'A') + 'A';
		directPass[i] = temp;
	}
	directPass[9] = '\0';
}
