#include <Arduino.h>
#include <CircuitOS.h>
#include <lvgl.h>
#include <BatController.h>
#include <BatCommon.h>
#include <SPIFFS.h>
#include "src/FSLVGL.h"
#include "src/InputLVGL.h"
#include <Loop/LoopManager.h>
#include <Com/Communication.h>
#include <WiFi.h>
#include "src/BatTheme.h"
#include "src/Screens/SimpleProg/SimpleProgScreen.h"
#include "src/Screens/IntroScreen.h"
#include "src/ShutdownService.h"
#include "src/LowBatteryService.h"

Display* display;
lv_disp_draw_buf_t drawBuffer;
static uint8_t drawData[160*20*2];

void lvglFlush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p){
	uint32_t w = (area->x2 - area->x1 + 1);
	uint32_t h = (area->y2 - area->y1 + 1);

	TFT_eSPI& tft = *display->getTft();
	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors(&color_p->full, w * h, true);
	tft.endWrite();
	lv_disp_flush_ready(disp);
}

void setup(){
	Serial.begin(115200);
	BatController.begin(false);

	display = BatController.getDisplay();
	auto sprite = display->getBaseSprite();
	sprite->resize(1, 1);

	pinMode(PIN_BATT, INPUT);
	srand(analogRead(PIN_BATT)*7+analogRead(PIN_BATT)*13);

	LoopManager::reserve(26);

	accelero.begin();
	vibro.begin();

	lv_init();
	lv_disp_draw_buf_init(&drawBuffer, drawData, nullptr, sizeof(drawData)/2);

	static lv_disp_drv_t displayDriver;
	lv_disp_drv_init(&displayDriver);
	displayDriver.hor_res = 160;
	displayDriver.ver_res = 128;
	displayDriver.flush_cb = lvglFlush;
	displayDriver.draw_buf = &drawBuffer;
	lv_disp_t * disp = lv_disp_drv_register(&displayDriver);
	BatThemeInit(disp);

	if(Battery.getPercentage() < 1 && !Battery.charging()){
		auto blank = new LVScreen();
		lv_obj_set_style_bg_color(blank->getLvObj(), lv_color_black(), 0);
		lv_obj_set_style_bg_opa(blank->getLvObj(), LV_OPA_COVER, 0);
		blank->start();

		auto modal = new LowBatteryModal(blank, BatType::Controller);
		modal->start();

		lv_timer_handler();
		BatController.fadeIn();

		delay(4000);
		BatController.shutdown();
	}

	new FSLVGL(SPIFFS, 'S');
	FSLVGL::loadCache();

	BatController.getInput()->addListener(new InputLVGL());
	SimpleProgScreen::touchIndex();

	auto intro = new IntroScreen();
	intro->setPreCallback([](){
		WiFi.begin();
		Com.begin();
	});
	intro->setPostCallback([](){
		AutoShutdown.begin();
		BatteryShutdown.begin();
	});
	intro->start();
	lv_timer_handler();
}

void loop(){
	LoopManager::loop();
	lv_timer_handler();
}
