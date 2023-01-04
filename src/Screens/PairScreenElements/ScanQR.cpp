#include "ScanQR.h"
#include <lvgl.h>
#include <Arduino.h>
#include <extra/libs/qrcode/lv_qrcode.h>
#include <extra/libs/qrcode/qrcodegen.h>

ScanQR::ScanQR(lv_obj_t* obj, lv_group_t* inputGroup) : inputGroup(inputGroup){
	scanQR = lv_obj_create(obj);
	lv_obj_set_style_bg_color(scanQR, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(scanQR, LV_OPA_COVER, 0);
	lv_obj_set_size(scanQR, 160, 128);
	lv_obj_set_pos(scanQR, 0, 0);
	lv_obj_add_flag(scanQR, LV_OBJ_FLAG_HIDDEN);
	lv_obj_set_scrollbar_mode(scanQR, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_all(scanQR, 5, 0);
	lv_obj_set_layout(scanQR, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(scanQR, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(scanQR, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_row(scanQR, Scale, 0);

	top = lv_label_create(scanQR);
	lv_obj_set_style_text_color(top, lv_color_white(), 0);
	lv_obj_set_style_text_font(top, &lv_font_unscii_8, 0);
	lv_label_set_long_mode(top, LV_LABEL_LONG_WRAP);

	qr = lv_qrcode_create(scanQR, 58, lv_color_white(), lv_color_black());

	bot = lv_label_create(scanQR);
	lv_obj_set_style_text_color(bot, lv_color_white(), 0);
	lv_obj_set_style_text_font(bot, &lv_font_unscii_8, 0);
	lv_label_set_long_mode(bot, LV_LABEL_LONG_WRAP);
	lv_label_set_text(bot, "Scan with Batmobile\nPress B to cancel");

	lv_obj_update_layout(scanQR);
	lv_obj_set_width(top, lv_obj_get_width(scanQR));
	lv_obj_set_width(bot, lv_obj_get_width(scanQR));
	lv_obj_set_style_pad_hor(top, 4, 0);
	lv_obj_set_style_pad_hor(bot, 4, 0);
	lv_obj_set_style_text_align(top, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_text_align(bot, LV_TEXT_ALIGN_CENTER, 0);
}

ScanQR::~ScanQR(){
	callback = nullptr;
	delete data;
}

void ScanQR::start(std::string ssid, std::string password, IPAddress ipAddress){
	lv_label_set_text(top, ("Connected to\n" + ssid).c_str());
	lv_obj_clear_flag(scanQR, LV_OBJ_FLAG_HIDDEN);

	lv_obj_add_event_cb(scanQR, [](lv_event_t* e){
		auto scanQR = static_cast<ScanQR*>(lv_event_get_user_data(e));

		if(scanQR->callback) scanQR->callback();
	}, LV_EVENT_CANCEL, this);

	lv_group_add_obj(inputGroup, scanQR);
	lv_group_focus_obj(scanQR);

	data = new char[48];
	uint8_t ssidLength = ssid.size();
	uint8_t passLength = password.size();

	for(int i = 0; i < 22; i++){
		i >= ssidLength ? data[i] = '\0' : data[i] = ssid[i];
		i >= passLength ? data[22 + i] = '\0' : data[22 + i] = password[i];
	}
	for(int i = 0; i < 4; i++){
		data[44 + i] = ipAddress[i];
	}

	lv_qrcode_update(qr, data, 48);
}

void ScanQR::stop(){
	lv_obj_add_flag(scanQR, LV_OBJ_FLAG_HIDDEN);
	lv_obj_remove_event_cb_with_user_data(scanQR, nullptr, this);
	lv_group_remove_obj(scanQR);
}

void ScanQR::setCallback(std::function<void()> cb){
	callback = cb;
}
