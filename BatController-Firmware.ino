#include <Arduino.h>
#include <CircuitOS.h>
#include <lvgl.h>
#include <BatController.h>
#include <BatCommon.h>
#include <SPIFFS.h>
#include <LVGL-cpp.h>
#include <Loop/LoopManager.h>
#include <Com/Communication.h>
#include <WiFi.h>
#include <FSLVGL.h>
#include <InputLVGL.h>
#include "src/BatTheme.h"
#include "src/Screens/SimpleProg/SimpleProgScreen.h"
#include "src/Screens/MainMenu.h"
#include "src/Screens/IntroScreen.h"
#include "src/ShutdownService.h"
#include "src/LowBatteryService.h"
#include "src/JigHWTest/JigHWTest.h"

Display* display;
lv_disp_draw_buf_t drawBuffer;
static uint8_t drawData[160 * 20 * 2];

bool checkJig(){
	char buf[7];
	int wp = 0;

	uint32_t start = millis();
	while(millis() - start < 500){
		while(Serial.available()){
			buf[wp] = Serial.read();
			wp = (wp + 1) % 7;

			for(int i = 0; i < 7; i++){
				int match = 0;
				static const char* target = "JIGTEST";

				for(int j = 0; j < 7; j++){
					match += buf[(i + j) % 7] == target[j];
				}

				if(match == 7) return true;
			}
		}
	}

	return false;
}

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

	{
		auto file = SPIFFS.open("/logo.raw");
		if(file){
			sprite->clear(TFT_BLACK);
			sprite->drawIcon(file, 60, 52, 41, 23);
			display->commit();

			BatController.setBrightness(150);
		}
	}

	if(checkJig()){
		BatController.fadeIn(0);
		printf("Jig\n");
		auto test = new JigHWTest(display);
		test->start();
		for(;;);
	}

	sprite->resize(1, 1);

	pinMode(PIN_BATT, INPUT);
	srand(analogRead(PIN_BATT) * 7 + analogRead(PIN_BATT) * 13);

	LoopManager::reserve(26);

	lv_init();
	lv_disp_draw_buf_init(&drawBuffer, drawData, nullptr, sizeof(drawData) / 2);

	static lv_disp_drv_t displayDriver;
	lv_disp_drv_init(&displayDriver);
	displayDriver.hor_res = 160;
	displayDriver.ver_res = 128;
	displayDriver.flush_cb = lvglFlush;
	displayDriver.draw_buf = &drawBuffer;
	lv_disp_t* disp = lv_disp_drv_register(&displayDriver);
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

	static const char* cached[] = {
			"/Menu/Small/Ball_b.bin",
			"/Menu/Small/Ball_r.bin",
			"/Menu/Small/SimpleProg_b.bin",
			"/Menu/Small/SimpleProg_r.bin",
			"/Menu/Small/Dance_b.bin",
			"/Menu/Small/Dance_r.bin",
			"/Menu/Small/Manual_b.bin",
			"/Menu/Small/Manual_r.bin",
			"/Menu/Small/Marker_b.bin",
			"/Menu/Small/Marker_r.bin",
			"/Menu/Small/Settings_b.bin",
			"/Menu/Small/Settings_r.bin",
			"/Menu/Label/Ball.bin",
			"/Menu/Label/SimpleProg.bin",
			"/Menu/Label/Dance.bin",
			"/Menu/Label/Manual.bin",
			"/Menu/Label/Marker.bin",
			"/Menu/Label/Settings.bin",
			// "/DriveScreen/ModalBg.bin",
			"/DriveScreen/BallHue.bin",
			"/DriveScreen/MarkerBackward.bin",
			"/DriveScreen/MarkerForward.bin",
			"/DriveScreen/MarkerRotate.bin",
			"/DriveScreen/MarkerBurnout.bin",
			"/DriveScreen/BallHuePicker.bin",
			"/DriveScreen/BallHueBar.bin",
			"/DriveScreen/SpeedBar.bin",
			"/DriveScreen/SpeedBarFill.bin",
			"/DriveScreen/Boost_full.bin",
			"/DriveScreen/Boost_empty.bin",
			"/DriveScreen/Boost_segment.bin",
			"/Signal/0.bin",
			"/Signal/1.bin",
			"/Signal/2.bin",
			"/Signal/3.bin",
			"/Battery/0.bin",
			"/Battery/1.bin",
			"/Battery/2.bin",
			"/Battery/3.bin",
			"/Battery/4.bin",
			"/Battery/5.bin",
			"/Battery/6.bin",
			"/Battery/7.bin",
			"/Battery/Batmobile.bin",
			"/Battery/Controller.bin",
			"/DriveMode/Ball.bin",
			"/DriveMode/Idle.bin",
			"/DriveMode/Line.bin",
			"/DriveMode/Marker.bin",
			"/DriveMode/Manual.bin",
			"/DriveMode/SimpleProg.bin"
	};

	new FSLVGL(SPIFFS, 'S', cached, sizeof(cached) / sizeof(cached[0]));
	FSLVGL::loadCache();
	static const std::map<uint8_t, lv_key_t> keyMap = {{ BTN_UP,    LV_KEY_PREV },
													   { BTN_DOWN,  LV_KEY_NEXT },
													   { BTN_LEFT,  LV_KEY_LEFT },
													   { BTN_RIGHT, LV_KEY_RIGHT },
													   { BTN_A,     LV_KEY_ENTER },
													   { BTN_B,     LV_KEY_ESC },
													   { BTN_MENU,  LV_KEY_HOME }};
	BatController.getInput()->addListener(new InputLVGL(keyMap));
	SimpleProgScreen::touchIndex();
	MainMenu::resetLastSelected();

	auto intro = new IntroScreen();
	intro->setPreCallback([](){
		WiFi.begin();
		Com.begin();
	});
	intro->setPostCallback([](){
		AutoShutdown.begin();
		BatteryShutdown.begin();
	});

	BatController.setBrightness(0);
	intro->start();
	lv_timer_handler();
}

void loop(){
	LoopManager::loop();
	lv_timer_handler();
}
