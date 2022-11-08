#include "BallHueElement.h"

BallHueElement::BallHueElement(lv_obj_t* parent, uint8_t hue) : LVObject(parent){
	lv_obj_add_flag(obj, LV_OBJ_FLAG_FLOATING);
	lv_obj_add_flag(obj, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_pos(obj, x, y);

	img = lv_img_create(obj);
	lv_img_set_src(img, "S:/DriveScreen/BallHue.bin");
	lv_obj_set_style_img_recolor(img, lv_color_hsv_to_rgb(hue * 2, 100, 100), LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor_opa(img, colorOpacity, LV_STATE_DEFAULT);

}

void BallHueElement::setHue(uint8_t hue){
	lv_obj_set_style_img_recolor(img, lv_color_hsv_to_rgb(hue * 2, 100, 100), LV_STATE_DEFAULT);
}
