#include "WiFiScanning.h"
#include <lvgl.h>
#include <WiFi.h>
#include <Loop/LoopManager.h>

WiFiScanning::WiFiScanning(lv_obj_t* obj, lv_group_t* inputGroup) : inputGroup(inputGroup){
	scanObj = lv_obj_create(obj);
	lv_obj_set_style_bg_color(scanObj, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(scanObj, LV_OPA_COVER, 0);
	lv_obj_set_size(scanObj, 160, 128);
	lv_obj_set_pos(scanObj, 0, 0);
	lv_obj_add_flag(scanObj, LV_OBJ_FLAG_HIDDEN);

	gif = lv_gif_create(scanObj);
	lv_gif_set_src(gif, "S:/Pairing/WiFiConnecting.gif");
	lv_obj_set_pos(gif, 59, 38);
	lv_obj_set_style_opa(gif, LV_OPA_COVER, 0);
	lv_obj_set_style_bg_opa(gif, LV_OPA_TRANSP, 0);
	lv_gif_set_loop(gif, LV_GIF_LOOP_ON);

	lv_obj_t* text = lv_label_create(scanObj);
	lv_obj_set_size(text, 80, 30);
	lv_obj_set_pos(text, 45, 80);
	lv_obj_set_style_text_color(text, lv_color_white(), 0);
	lv_obj_set_style_text_font(text, &lv_font_unscii_8, 0);
	lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(text, "Scanning");

	listObj = lv_obj_create(obj);
	lv_obj_set_size(listObj, 160, 128);
	lv_obj_set_pos(listObj, 0, 0);
	lv_obj_add_flag(listObj, LV_OBJ_FLAG_HIDDEN);

	lv_obj_t* header = lv_label_create(listObj);
	lv_obj_set_size(header, 160, 18);
	lv_obj_set_pos(header, 0, 0);
	lv_label_set_text(header, "Scanned networks");
	lv_obj_set_align(header, LV_ALIGN_TOP_MID);
	lv_obj_set_style_text_color(header, lv_color_white(), 0);
	lv_obj_set_style_text_font(header, &lv_font_unscii_8, 0);
	lv_obj_set_style_text_align(header, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, 0);
	lv_obj_set_style_border_width(header, 2, 0);
	lv_obj_set_style_border_color(header, lv_color_white(), 0);
	lv_obj_set_style_border_opa(header, LV_OPA_COVER, 0);

	list = lv_obj_create(listObj);
	lv_obj_set_style_bg_color(list, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(list, LV_OPA_COVER, 0);
	lv_obj_set_size(list, 160, 110);
	lv_obj_set_pos(list, 0, 18);
	lv_obj_set_layout(list, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(list, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_row(list, 3, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(list, 5, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(list, 3, LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);
}

WiFiScanning::~WiFiScanning(){
	callbackDone = nullptr;
	callbackBack = nullptr;
	callbackError = nullptr;
}

void WiFiScanning::start(){
	startScan();
}

void WiFiScanning::stop(){
	lv_gif_stop(gif);
	lv_group_remove_all_objs(inputGroup);
	lv_obj_add_flag(listObj, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(scanObj, LV_OBJ_FLAG_HIDDEN);
	WiFi.scanDelete();
}


void WiFiScanning::loop(uint micros){
	counter += micros;

	int networks = WiFi.scanComplete();
	bool done = networks != -1;

	if(counter >= Timeout || (done && networks <= 0))
	{
		LoopManager::removeListener(this);
		counter = 0;
		if(!callbackError) return;
		callbackError("No networks found\n\nPress any key.");
		return;
	}else if(done && networks > 0){
		LoopManager::removeListener(this);
		counter = 0;
		numOfNetworks = networks;
		startList();
		return;
	}
}

void WiFiScanning::setCallbackBack(std::function<void()> cb){
	callbackBack = cb;
}

void WiFiScanning::setCallbackDone(std::function<void(std::string)> cb){
	callbackDone = cb;
}

void WiFiScanning::setCallbackError(std::function<void(std::string)> cb){
	callbackError = cb;
}

void WiFiScanning::startScan(){
	lv_obj_add_flag(listObj, LV_OBJ_FLAG_HIDDEN);

	lv_gif_restart(gif);
	lv_gif_start(gif);
	lv_obj_clear_flag(scanObj, LV_OBJ_FLAG_HIDDEN);

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	WiFi.scanNetworks(true);
	LoopManager::addListener(this);
}

void WiFiScanning::startList(){
	lv_gif_stop(gif);
	lv_obj_add_flag(scanObj, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(listObj, LV_OBJ_FLAG_HIDDEN);

	for(int j = lv_obj_get_child_cnt(list) - 1; j >= 0; --j){
		lv_obj_del(lv_obj_get_child(list, j));
	}

	for(int i = 0; i < numOfNetworks; i++){

		lv_obj_t* listElement = lv_btn_create(list);
		lv_group_add_obj(inputGroup, listElement);
		lv_obj_set_size(listElement, ElementWidth, ElementHeight);

		lv_obj_set_style_outline_width(listElement, 1, LV_STATE_FOCUSED);
		lv_obj_set_style_outline_opa(listElement, LV_OPA_COVER, LV_STATE_FOCUSED);
		lv_obj_set_style_outline_color(listElement, lv_color_white(), LV_STATE_FOCUSED);
		lv_obj_set_style_radius(listElement, 3, LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(listElement, LV_TEXT_ALIGN_CENTER, 0);

		lv_obj_t* label = lv_label_create(listElement);
		lv_label_set_text(label, WiFi.SSID(i).c_str());
		lv_obj_center(label);
		lv_obj_set_size(label, lv_pct(100), lv_pct(80));
		lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(listElement, lv_color_white(), LV_STATE_DEFAULT);

		lv_obj_add_event_cb(listElement, [](lv_event_t* e){
			lv_obj_t* label = (lv_obj_t*) lv_event_get_user_data(e);
			lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
		}, LV_EVENT_FOCUSED, label);

		lv_obj_add_event_cb(listElement, [](lv_event_t* e){
			lv_obj_t* label = (lv_obj_t*) lv_event_get_user_data(e);
			lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
		}, LV_EVENT_DEFOCUSED, label);

		lv_obj_add_event_cb(listElement, [](lv_event_t* e){
			auto scanning = (WiFiScanning*) e->user_data;
			auto index = lv_obj_get_index(lv_group_get_focused(scanning->inputGroup));
			if(!scanning->callbackDone) return;
			scanning->callbackDone(std::string("")+ WiFi.SSID(index).c_str());
		}, LV_EVENT_CLICKED, this);

		lv_obj_add_event_cb(listElement, [](lv_event_t* e){
			auto key = lv_event_get_key(e);
			if(key == LV_KEY_ESC || key == LV_KEY_HOME){
				auto scanning = (WiFiScanning*) e->user_data;
				if(!scanning->callbackBack) return;
				scanning->callbackBack();
			}
		},LV_EVENT_KEY, this);
	}

	lv_group_set_focus_cb(inputGroup, [](lv_group_t* g){
		lv_obj_scroll_to_view(lv_group_get_focused(g), LV_ANIM_ON);
	});
	lv_group_focus_obj(lv_obj_get_child(list, 0));
}
