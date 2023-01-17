#include "LowBatteryModal.h"

LowBatteryModal::LowBatteryModal(LVScreen *parent, BatType batType) : LVModal(parent) {
    lv_obj_set_size(obj, w, h);
    lv_obj_set_style_bg_img_src(obj, "S:/DriveScreen/ModalBg.bin", LV_STATE_DEFAULT);
    lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(obj, 5, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(obj, 7, LV_STATE_DEFAULT);

    text = lv_img_create(obj);
    lv_obj_set_size(text, 65, 18);
    lv_img_set_src(text, batType == BatType::Batmobile ?
        "S:/LowBattery/VLowBat.bin" : "S:/LowBattery/CLowBat.bin");

    img = lv_img_create(obj);
    lv_obj_set_size(img, 29, 12);
    lv_img_set_src(img, "S:/LowBattery/LowBat.bin");
}