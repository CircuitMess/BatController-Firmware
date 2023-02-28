#include <stdlib.h>
#include <core/lv_obj.h>
#include "ScanAruco.h"
#include <lvgl.h>
#include <Display/Color.h>
#include <Aruco/Aruco.h>
#include <BatController.h>
#include <Settings.h>

ScanAruco::ScanAruco(lv_obj_t* obj, lv_group_t* inputGroup) : inputGroup(inputGroup){
	scanAruco = lv_obj_create(obj);
	lv_obj_set_style_bg_color(scanAruco, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(scanAruco, LV_OPA_COVER, 0);
	lv_obj_set_size(scanAruco, 160, 128);
	lv_obj_set_pos(scanAruco, 0, 0);
	lv_obj_add_flag(scanAruco, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_scrollbar_mode(scanAruco, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_all(scanAruco, 5, 0);
	lv_obj_set_layout(scanAruco, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(scanAruco, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(scanAruco, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_row(scanAruco, 7, 0);
	lv_obj_clear_flag(scanAruco, LV_OBJ_FLAG_SCROLLABLE);

	buffer = static_cast<lv_color_t*>(malloc(sizeof(lv_color_t) * 49 * Scale * Scale));

	top = lv_label_create(scanAruco);
	lv_obj_set_width(top, lv_pct(100));
	lv_obj_set_style_text_align(top, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_text_color(top, lv_color_white(), 0);
	lv_obj_set_style_text_font(top, &lv_font_unscii_8, 0);
	lv_label_set_long_mode(top, LV_LABEL_LONG_WRAP);
	lv_label_set_text(top, "Scan with\nBatmobile to pair");

	canvas = lv_canvas_create(scanAruco);
	lv_obj_set_size(canvas, ArucoSize * Scale, ArucoSize * Scale);
	lv_obj_set_style_bg_color(canvas, lv_palette_darken(LV_PALETTE_BLUE, 2), 0);
	lv_obj_set_style_bg_opa(canvas, LV_OPA_COVER, 0);
	lv_canvas_set_buffer(canvas, buffer, ArucoSize * Scale, ArucoSize * Scale, LV_IMG_CF_INDEXED_1BIT);
	lv_canvas_set_palette(canvas, 0, lv_color_white());
	lv_canvas_set_palette(canvas, 1, lv_color_black());

	white.full = 0;
	black.full = 1;
	lv_canvas_fill_bg(canvas, black, LV_OPA_COVER);

	lv_obj_update_layout(scanAruco);

}

ScanAruco::~ScanAruco(){
	free(buffer);
	callback = nullptr;
}


void ScanAruco::start(uint16_t id){
	Color tempBuffer[7 * 7];

	lv_obj_clear_flag(scanAruco, LV_OBJ_FLAG_HIDDEN);

	Aruco::generate(id, tempBuffer);
	auto drawRect = [this](uint8_t x, uint8_t y){
		for(uint32_t locY = y * Scale; locY < y * Scale + Scale; locY++){
			for(uint32_t locX = x * Scale; locX < x * Scale + Scale; locX++){
				lv_canvas_set_px(this->canvas, locX, locY, this->white);
			}
		}
	};

	lv_canvas_fill_bg(this->canvas, this->black, LV_OPA_COVER);
	for(uint8_t y = 0; y < 7; y++){
		for(uint8_t x = 0; x < 7; x++){
			if(tempBuffer[y * 7 + x] == 0x0000){
				drawRect(x, y);
			}
		}
	}

	lv_obj_add_event_cb(scanAruco, [](lv_event_t* e){
		ScanAruco* scanA = static_cast<ScanAruco*>(lv_event_get_user_data(e));

		if(!scanA->callback) return;
		scanA->callback();
	}, LV_EVENT_RELEASED, this);

	lv_group_add_obj(inputGroup, scanAruco);
	lv_group_focus_obj(scanAruco);

	BatController.setBrightness(40);
}

void ScanAruco::stop(){
	lv_obj_add_flag(scanAruco, LV_OBJ_FLAG_HIDDEN);
	lv_obj_remove_event_cb_with_user_data(scanAruco, nullptr, this);
	lv_group_remove_obj(scanAruco);

	BatController.setBrightness(Settings.get().screenBrightness);
}

void ScanAruco::setCallback(std::function<void()> cb){
	callback = cb;
}