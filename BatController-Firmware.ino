#include <Arduino.h>
#include <CircuitOS.h>
#include <lvgl.h>
#include <BatController.h>
#include <SPIFFS.h>
#include "src/FSLVGL.h"
#include "src/InputLVGL.h"
#include <Loop/LoopManager.h>
#include "src/BatTheme.h"
#include "src/Feed/Feed.h"


lv_disp_draw_buf_t drawBuffer;
Display* display;

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
	BatController.begin();

	display = BatController.getDisplay();

	LoopManager::reserve(10);

	lv_init();
	if(!display->getBaseSprite()->created()){
		Serial.println("not created");
		return;
	}
	lv_disp_draw_buf_init(&drawBuffer, display->getBaseSprite()->getBuffer(), NULL, 160 * 128);

	new FSLVGL(SPIFFS, 'S');

	static lv_disp_drv_t displayDriver;
	lv_disp_drv_init(&displayDriver);
	displayDriver.hor_res = 160;
	displayDriver.ver_res = 128;
	displayDriver.flush_cb = lvglFlush;
	displayDriver.draw_buf = &drawBuffer;
	lv_disp_t* disp = lv_disp_drv_register(&displayDriver);
//	BatThemeInit(disp);

	BatController.getInput()->addListener(new InputLVGL());

	static lv_obj_t* scr = lv_obj_create(nullptr);
	lv_scr_load(scr);

	uint8_t* buf = (uint8_t*) malloc(160 * 120 * 2);

	lv_obj_t* canvas = lv_canvas_create(scr);
	lv_canvas_set_buffer(canvas, buf, 160, 120, LV_IMG_CF_RAW);


	Feed* feed = new Feed();
	feed->onFrame([canvas](const DriveInfo& info, const Color* frame){

		static lv_img_dsc_t my_img_dsc;

		my_img_dsc.data_size = info.jpgSize;
		my_img_dsc.data = info.jpg;

		my_img_dsc.header.cf = LV_IMG_CF_RAW;
		my_img_dsc.header.always_zero = 0;
		my_img_dsc.header.w = 160;
		my_img_dsc.header.h = 120;

		static lv_draw_img_dsc_t* drawDescriptor;
		lv_draw_img_dsc_init(drawDescriptor);

		lv_canvas_draw_img(canvas, 0,0, &my_img_dsc, drawDescriptor);
	});
}

void loop(){
	lv_timer_handler();
	LoopManager::loop();
}
