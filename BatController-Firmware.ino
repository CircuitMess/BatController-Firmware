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
#include "src/Screens/IntroScreen.h"
#include "src/LowBatteryService.h"

lv_disp_draw_buf_t drawBuffer;
Display* display;
LowBatteryService* lowBatteryService;

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

	pinMode(PIN_BATT, INPUT);
	srand(analogRead(PIN_BATT)*7+analogRead(PIN_BATT)*13);

	display = BatController.getDisplay();

	LoopManager::reserve(10);

	lv_init();
	if(!display->getBaseSprite()->created()){
		Serial.println("not created");
		return;
	}
	lv_disp_draw_buf_init(&drawBuffer, display->getBaseSprite()->getBuffer(), NULL, 160 * 128);

	new FSLVGL(SPIFFS, 'S');
	FSLVGL::loadCache();

	static lv_disp_drv_t displayDriver;
	lv_disp_drv_init(&displayDriver);
	displayDriver.hor_res = 160;
	displayDriver.ver_res = 128;
	displayDriver.flush_cb = lvglFlush;
	displayDriver.draw_buf = &drawBuffer;
	lv_disp_t * disp = lv_disp_drv_register(&displayDriver);
//	BatThemeInit(disp);

	BatController.getInput()->addListener(new InputLVGL());

    lowBatteryService = new LowBatteryService();

    if(lowBatteryService->isBatteryLow()){
        BatController.setBrightness(75);
        auto blackScreen = new LVScreen();
        lv_obj_set_style_bg_color(blackScreen->getLvObj(), lv_color_black(), 0);
        lv_obj_set_style_bg_opa(blackScreen->getLvObj(), LV_OPA_COVER, 0);
        blackScreen->start();
        lowBatteryService->begin();
        return;
    }

    auto intro = new IntroScreen([]() {
        WiFi.begin();
        Com.begin();
    });
    intro->start();
    lowBatteryService->begin();

}

void loop(){
	LoopManager::loop();
	lv_timer_handler();
}
