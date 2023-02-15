#include "DanceElement.h"


DanceElement::DanceElement(lv_obj_t *parent) : LVObject(parent) {
    lv_obj_set_size(obj, 110, 20);
    lv_obj_set_pos(obj, 25, 95);

    arrowL = lv_img_create(obj);
    lv_img_set_src(arrowL, LV_SYMBOL_LEFT);
    lv_obj_align(arrowL, LV_ALIGN_LEFT_MID, 2, 0);
    lv_obj_set_style_opa(arrowL, LV_OPA_100, 0);

    arrowR = lv_img_create(obj);
    lv_img_set_src(arrowR, LV_SYMBOL_RIGHT);
    lv_obj_align(arrowR, LV_ALIGN_RIGHT_MID, -2, 0);
    lv_obj_set_style_opa(arrowR, LV_OPA_100, 0);

    img = lv_img_create(obj);
    lv_img_set_src(img, "S:/DriveScreen/Board.bin");
    lv_obj_center(img);

    currentDance = lv_label_create(obj);
    lv_obj_center(currentDance);
    lv_obj_set_style_text_color(currentDance, lv_color_white(), 0);
    lv_obj_set_style_text_font(currentDance, &lv_font_unscii_8, 0);

}

void DanceElement::setCurrentDance(DanceType danceType) {
    lv_label_set_text(currentDance, dances[(uint8_t)danceType].c_str());
}

void DanceElement::arrowLeftPressed(bool pressed) {
    lv_obj_set_style_text_color(arrowL, pressed ? lv_color_white() : lv_color_black(), 0);
}

void DanceElement::arrowRightPressed(bool pressed) {
    lv_obj_set_style_text_color(arrowR, pressed ? lv_color_white() : lv_color_black(), 0);
}
