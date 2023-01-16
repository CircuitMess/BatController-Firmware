#include "LowBatteryModal.h"
#include <font/lv_font.h>

LowBatteryModal::LowBatteryModal(LVScreen *parent, BatType batType) : LVModal(parent) {
    lv_obj_set_size(obj, w, h);
    lv_obj_set_style_bg_img_src(obj, "S:/DriveScreen/ModalBg.bin", LV_STATE_DEFAULT);
    lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(obj, 3, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(obj, 5, LV_STATE_DEFAULT);

    img = lv_img_create(obj);
    lv_img_set_src(img, "S:/Battery/0.bin");
    lv_obj_set_size(img, 12, 5);

    text = lv_label_create(obj);
    lv_obj_set_size(text, w-8, h);
    lv_obj_set_style_pad_top(text, 5, 0);
    lv_obj_set_style_text_font(text, &lv_font_montserrat_8 , 0);
    lv_obj_set_style_text_color(text, lv_color_black(), 0);
    lv_label_set_text(text, batType == BatType::Controller ?  "Controller battery low, shutting \ndown" : "Batmobile battery low, shutting \ndown");
}