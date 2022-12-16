
#include "ScanQR.h"
#include <lvgl.h>
#include <Arduino.h>
#include <extra/libs/qrcode/lv_qrcode.h>

ScanQR::ScanQR(lv_obj_t *obj, lv_group_t *inputGroup) : inputGroup(inputGroup) {
    scanQR = lv_obj_create(obj);
    lv_obj_set_style_bg_color(scanQR, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(scanQR, LV_OPA_COVER, 0);
    lv_obj_set_size(scanQR, 160, 128);
    lv_obj_set_pos(scanQR, 0, 0);
    lv_obj_add_flag(scanQR, LV_OBJ_FLAG_HIDDEN);

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
    lv_label_set_text(title, "Connected to...");

    qr = lv_qrcode_create(scanQR, 58, lv_color_white(), lv_color_black());
    lv_obj_set_pos(qr, 10, 45);
}

ScanQR::~ScanQR() {
    callback = nullptr;
}

void ScanQR::start(std::string ssid, std::string password, IPAddress ipAddress) {
    lv_label_set_text(title, ("Connected to\n" + ssid).c_str());
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
