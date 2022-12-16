
#include "ScanQR.h"
#include <lvgl.h>
#include <Arduino.h>

ScanQR::ScanQR(lv_obj_t *obj, lv_group_t *inputGroup) : inputGroup(inputGroup) {
    scanQR = lv_obj_create(obj);
    lv_obj_set_style_bg_color(scanQR, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(scanQR, LV_OPA_COVER, 0);
    lv_obj_set_size(scanQR, 160, 128);
    lv_obj_set_pos(scanQR, 0, 0);
    lv_obj_add_flag(scanQR, LV_OBJ_FLAG_HIDDEN);

    buffer = static_cast<lv_color_t *>(malloc(sizeof(lv_color_t) * 58 * 58));

    canvas = lv_canvas_create(scanQR);
    lv_obj_set_size(canvas, 58, 58);
    lv_obj_set_pos(canvas, 10, 50);
    lv_obj_set_style_bg_color(canvas, lv_palette_darken(LV_PALETTE_BLUE, 2), 0);
    lv_obj_set_style_bg_opa(canvas, LV_OPA_COVER, 0);
    lv_canvas_set_buffer(canvas, buffer, 58, 58, LV_IMG_CF_INDEXED_1BIT);
    lv_canvas_set_palette(canvas, 0, lv_color_white());
    lv_canvas_set_palette(canvas, 1, lv_color_black());
    lv_color_t white;
    white.full = 0;

    lv_canvas_fill_bg(canvas, white, LV_OPA_COVER);

    text = lv_label_create(scanQR);
    lv_obj_set_size(text, Multiplier * Scale, Multiplier * Scale);
    lv_obj_set_pos(text, Multiplier * Scale, 40);
    lv_obj_set_style_text_color(text, lv_color_white(), 0);
    lv_obj_set_style_text_font(text, &lv_font_unscii_8, 0);
    lv_label_set_text(text, "Scan with\ncamera\nto begin\npairing\nor press\nA to use\ndirect connection");

    title = lv_label_create(scanQR);
    lv_obj_set_size(title, 150, 15);
    lv_obj_set_pos(title, 5, 5);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_unscii_8, 0);
    lv_label_set_text(title, "Connected to..."); //TODO add a network and save it
}

ScanQR::~ScanQR() {
    callback = nullptr;
}

void ScanQR::start(String networkName) {
    lv_label_set_text(title, ("Connected to " + networkName).c_str());
    lv_obj_clear_flag(scanQR, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_event_cb(scanQR, [](lv_event_t* e){
        if (lv_indev_get_key(lv_indev_get_act()) != LV_KEY_ENTER) return;
        ScanQR *scanQR = static_cast<ScanQR *>(lv_event_get_user_data(e));

        if (!scanQR->callback) return;
        scanQR->callback();
    }, LV_EVENT_RELEASED, this);

    lv_group_add_obj(inputGroup, scanQR);
    lv_group_focus_obj(scanQR);
}

void ScanQR::stop() {
    lv_obj_add_flag(scanQR, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_event_cb_with_user_data(scanQR, nullptr, this);
    lv_group_remove_obj(scanQR);
}

void ScanQR::setCallback(std::function<void()> cb) {
    callback = cb;
}
