#include "PairScreen.h"
#include <lvgl.h>
#include <BatController.h>
#include <Aruco/Aruco.h>

PairScreen::PairScreen() : LVScreen(){
	lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);

	bg = lv_obj_create(obj);
	lv_obj_set_style_bg_color(bg, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(bg, LV_OPA_COVER, 0);
	lv_obj_set_size(bg, 160, 128);
	lv_obj_set_pos(bg, 0, -128);

	buffer = static_cast<lv_color_t*>(malloc(sizeof(lv_color_t) * 49 * scale * scale));

	canvas = lv_canvas_create(bg);
	lv_obj_set_pos(canvas, 10, (128 - 7 * scale) / 2);
	lv_obj_set_size(canvas, 7 * scale, 7 * scale);
	lv_obj_set_style_bg_color(canvas, lv_palette_darken(LV_PALETTE_BLUE, 2), 0);
	lv_obj_set_style_bg_opa(canvas, LV_OPA_COVER, 0);
	lv_canvas_set_buffer(canvas, buffer, 7 * scale, 7 * scale, LV_IMG_CF_INDEXED_1BIT);
	lv_canvas_set_palette(canvas, 0, lv_color_white());
	lv_canvas_set_palette(canvas, 1, lv_color_black());

	white.full = 0;
	black.full = 1;
	lv_canvas_fill_bg(canvas, black, LV_OPA_COVER);

	text = lv_label_create(bg);
	lv_obj_set_size(text, 7*scale, 7*scale);
	lv_obj_set_pos(text, 7 * scale + 20, (128 - 7 * scale) / 2);
	lv_obj_set_style_text_color(text, lv_color_white(), 0);
	lv_obj_set_style_text_font(text, &lv_font_unscii_8 ,0);
	lv_label_set_text(text, "Scan\nwith\ncamera\nto begin\npairing");

	anim_drop = new lv_anim_t();
	lv_anim_init(anim_drop);
	lv_anim_set_var(anim_drop, bg);
	lv_anim_set_values(anim_drop, -128, 0);
	lv_anim_set_time(anim_drop, 500);
	lv_anim_set_repeat_count(anim_drop, 0);
	lv_anim_set_exec_cb(anim_drop, [](void* var, int32_t v){
		lv_obj_set_y((_lv_obj_t*) var, v);
	});

}


PairScreen::~PairScreen(){
	free(buffer);
}

void PairScreen::onStart(){
	int randID = rand() % 256;
	Color tempBuffer[7 * 7];

	Aruco::generate(randID, tempBuffer);
	auto drawRect = [this](uint8_t x, uint8_t y){
		for(uint32_t locY = y * scale; locY < y * scale + scale; locY++){
			for(uint32_t locX = x * scale; locX < x * scale + scale; locX++){
				lv_canvas_set_px(this->canvas, locX, locY, this->white);
			}
		}
	};

	for(uint8_t y = 0; y < 7; y++){
		for(uint8_t x = 0; x < 7; x++){
			if(tempBuffer[y * 7 + x] == TFT_BLACK){
				drawRect(x, y);
			}
		}
	}

	lv_anim_start(anim_drop);
}

void PairScreen::onStop(){
	LVScreen::onStop();
}

