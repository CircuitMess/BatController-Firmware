#include "Connecting.h"
#include <lvgl.h>

Connecting::Connecting(lv_obj_t* obj){
	connecting = lv_obj_create(obj);
	lv_obj_set_style_bg_color(connecting, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(connecting, LV_OPA_COVER, 0);
	lv_obj_set_size(connecting, 160, 128);
	lv_obj_set_pos(connecting, 0, 0);
	lv_obj_add_flag(connecting, LV_OBJ_FLAG_HIDDEN);

	gif = lv_gif_create(connecting);
	lv_gif_set_src(gif, "S:/Pairing/WiFiConnecting.gif");
	lv_obj_set_pos(gif, 59, 38);
	lv_obj_set_style_opa(gif, LV_OPA_COVER, 0);
	lv_obj_set_style_bg_opa(gif, LV_OPA_TRANSP, 0);
	lv_gif_set_loop(gif, LV_GIF_LOOP_ON);

	text = lv_label_create(connecting);
	lv_obj_set_size(text, 80, 30);
	lv_obj_set_pos(text, 40, 80);
	lv_obj_set_style_text_color(text, lv_color_white(), 0);
	lv_obj_set_style_text_font(text, &lv_font_unscii_8, 0);
	lv_label_set_text(text, "Connecting");
}

void Connecting::start(){
	lv_gif_restart(gif);
	lv_gif_start(gif);
	lv_obj_clear_flag(connecting, LV_OBJ_FLAG_HIDDEN);
}

void Connecting::stop(){
	lv_obj_add_flag(connecting, LV_OBJ_FLAG_HIDDEN);
	lv_gif_stop(gif);
}
