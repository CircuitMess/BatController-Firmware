#include "SettingsScreen.h"
#include <Settings.h>
#include <string>
#include <Pins.hpp>
#include <BatController.h>
#include <Audio/Piezo.h>
#include <nvs_flash.h>
#include <Com/Communication.h>
#include "PairScreen.h"
#include "../InputLVGL.h"

SettingsScreen::SettingsScreen() : LVScreen(), factoryResetPrompt(this, "Are you sure?\n\nThis will erase ALL data!"){


	lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);

	lv_style_selector_t sel = LV_PART_MAIN | LV_STATE_DEFAULT;
	lv_style_selector_t selFocus = LV_PART_MAIN | LV_STATE_FOCUSED;
	lv_style_selector_t selPress = LV_STATE_EDITED;

	lv_style_init(&style_def);
	lv_style_set_border_width(&style_def, 1);
	lv_style_set_border_opa(&style_def, 0);

	lv_style_init(&style_focused);
	lv_style_set_border_width(&style_focused, 1);
	lv_style_set_border_color(&style_focused, lv_color_white());
	lv_style_set_border_opa(&style_focused, LV_OPA_COVER);

	lv_style_init(&style_pressed);
	lv_style_set_border_width(&style_pressed, 1);
	lv_style_set_border_color(&style_pressed, lv_color_hex(0x892eff));
	lv_style_set_border_opa(&style_pressed, LV_OPA_COVER);


	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_height(obj, lv_pct(100));


	//Title
	title = lv_obj_create(obj);
	lv_obj_set_height(title, LV_SIZE_CONTENT);
	lv_obj_set_layout(title, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(title, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(title, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(title, 8, 0);
	lv_obj_set_style_pad_all(title, 3, 0);
	lv_obj_set_style_bg_opa(title, 0, 0);


	lv_obj_t* titleLabel = lv_label_create(title);
	lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_color(titleLabel, lv_color_white(), 0);
	lv_label_set_text(titleLabel, "Batcontroller Settings");

	lv_obj_update_layout(title);

	//scrollable container
	flexContainer = lv_obj_create(obj);
	lv_obj_set_width(flexContainer, lv_pct(100));
	lv_obj_set_height(flexContainer, lv_obj_get_height(obj) - lv_obj_get_height(title));
	lv_obj_set_layout(flexContainer, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(flexContainer, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(flexContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(flexContainer, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_gap(flexContainer, 2, 0);
	lv_obj_set_style_pad_all(flexContainer, 3, 0);

	//sound slider
	sound = lv_obj_create(flexContainer);
	lv_obj_set_height(sound, LV_SIZE_CONTENT);
	lv_obj_set_width(sound, lv_pct(100));
	lv_obj_set_layout(sound, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(sound, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(sound, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all(sound, 3, 0);
	lv_obj_set_style_bg_opa(sound, 0, 0);
	lv_obj_add_style(sound, &style_pressed, selPress);
	lv_obj_add_style(sound, &style_focused, selFocus);
	lv_obj_add_style(sound, &style_def, sel);

	lv_obj_t* soundLabel = lv_label_create(sound);
	lv_obj_set_style_text_font(soundLabel, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_color(soundLabel, lv_color_white(), 0);
	lv_label_set_text(soundLabel, "Sound");


	soundSlider = lv_slider_create(sound);
	lv_obj_add_flag(soundSlider, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	setButtonHoldAndRepeatTime(BTN_LEFT, 50);
	setButtonHoldAndRepeatTime(BTN_RIGHT, 50);
	setButtonHoldTime(BTN_LEFT, 400);
	setButtonHoldTime(BTN_RIGHT, 400);

	lv_slider_set_range(soundSlider, 0, 50);
	lv_obj_remove_style_all(soundSlider);        /*Remove the styles coming from the theme*/
	lv_obj_set_size(soundSlider, 56, 12);
	lv_obj_set_style_pad_hor(soundSlider, 5, 0);

	lv_obj_add_event_cb(soundSlider, [](lv_event_t* event){
		lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
		auto screen = static_cast<SettingsScreen*>(event->user_data);
		lv_obj_scroll_to_y(screen->flexContainer, 0, LV_ANIM_ON);
	}, LV_EVENT_FOCUSED, this);

	lv_obj_add_event_cb(soundSlider, [](lv_event_t* event){
		lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_DEFOCUSED, this);

	lv_obj_add_event_cb(soundSlider, [](lv_event_t* event){
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		if(!(lv_obj_get_state(event->target) & LV_STATE_EDITED)){
			slider->backToMain();
		}
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(soundSlider, [](lv_event_t* event){
		if(lv_obj_get_state(event->target) & LV_STATE_EDITED){
			lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}else{
			lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}
	}, LV_EVENT_STYLE_CHANGED, nullptr);

	lv_obj_add_event_cb(soundSlider, [](lv_event_t* event){
		if(lv_event_get_key(event) != LV_KEY_HOME) return;
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		slider->backToMain();
	}, LV_EVENT_KEY, this);

	lv_obj_add_event_cb(soundSlider, [](lv_event_t* event){
		lv_obj_t* slider = static_cast<lv_obj_t*>(event->user_data);
		lv_group_t* group = static_cast<lv_group_t*>(lv_obj_get_group(slider));
		if(group == nullptr){
			return;
		}
		lv_event_send(slider, LV_EVENT_RELEASED, nullptr);
		lv_event_send(slider, LV_EVENT_SHORT_CLICKED, nullptr);
		lv_event_send(slider, LV_EVENT_CLICKED, nullptr);
		lv_group_send_data(group, LV_KEY_ENTER);
	}, LV_EVENT_CANCEL, soundSlider);

	lv_group_add_obj(inputGroup, soundSlider);

	lv_obj_add_event_cb(soundSlider, [](lv_event_t* event){
		lv_obj_t* slider = static_cast<lv_obj_t*>(event->user_data);
		Com.sendVolume(lv_slider_get_value(slider) * 2);
		Com.sendSettingsSound();
		Settings.get().soundVolume = lv_slider_get_value(slider) * 2;
	}, LV_EVENT_VALUE_CHANGED, soundSlider);

	lv_style_init(&style_main);
	lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
	lv_style_set_bg_color(&style_main, lv_color_hex3(0xbbb));
	lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
	lv_obj_add_style(soundSlider, &style_main, LV_PART_MAIN);

	lv_style_init(&style_knob);
	lv_style_set_bg_opa(&style_knob, LV_OPA_100);
	lv_obj_add_style(soundSlider, &style_knob, LV_PART_KNOB);
	lv_style_set_text_font(&style_knob, &lv_font_montserrat_10);
	lv_style_set_text_color(&style_knob, lv_color_white());
	lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
	lv_style_set_height(&style_knob, 10);
	lv_style_set_width(&style_knob, 10);
	lv_obj_add_style(soundSlider, &style_knob, LV_PART_KNOB | LV_STATE_EDITED);

	//shutdownTime
	shutdownTime = lv_obj_create(flexContainer);
	lv_obj_set_height(shutdownTime, LV_SIZE_CONTENT);
	lv_obj_set_width(shutdownTime, lv_pct(100));
	lv_obj_set_layout(shutdownTime, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(shutdownTime, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(shutdownTime, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all(shutdownTime, 3, 0);
	lv_obj_set_style_bg_opa(shutdownTime, 0, 0);
	lv_obj_add_style(shutdownTime, &style_pressed, selPress);
	lv_obj_add_style(shutdownTime, &style_focused, selFocus);
	lv_obj_add_style(shutdownTime, &style_def, sel);

	lv_obj_t* shutdownLabel = lv_label_create(shutdownTime);
	lv_obj_set_style_text_font(shutdownLabel, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_color(shutdownLabel, lv_color_white(), 0);
	lv_obj_set_style_pad_top(shutdownLabel, 2, 0);
	lv_label_set_text(shutdownLabel, "Shutdown time");

	shutdownSlider = lv_slider_create(shutdownTime);
	lv_obj_add_flag(shutdownSlider, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_FOCUSED, nullptr);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_DEFOCUSED, nullptr);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		if(!(lv_obj_get_state(event->target) & LV_STATE_EDITED)){
			slider->backToMain();
		}
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		if(lv_event_get_key(event) != LV_KEY_HOME) return;
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		slider->backToMain();
	}, LV_EVENT_KEY, this);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		auto* settings = static_cast<SettingsScreen*>(event->user_data);

		lv_label_set_text(settings->shutdownTimeLabel, ShutdownText[lv_slider_get_value(event->target)]);
		Settings.get().shutdownTime = lv_slider_get_value(event->target);
	}, LV_EVENT_VALUE_CHANGED, this);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		lv_obj_t* slider = static_cast<lv_obj_t*>(event->user_data);
		lv_group_t* group = static_cast<lv_group_t*>(lv_obj_get_group(slider));
		if(group == nullptr){
			return;
		}
		lv_event_send(slider, LV_EVENT_RELEASED, nullptr);
		lv_event_send(slider, LV_EVENT_SHORT_CLICKED, nullptr);
		lv_event_send(slider, LV_EVENT_CLICKED, nullptr);
		lv_group_send_data(group, LV_KEY_ENTER);
	}, LV_EVENT_CANCEL, shutdownSlider);

	lv_group_add_obj(inputGroup, shutdownSlider);

	lv_slider_set_range(shutdownSlider, 0, SHUTDOWN_STEPS - 1);
	lv_obj_remove_style_all(shutdownSlider);        /*Remove the styles coming from the theme*/
	lv_obj_set_size(shutdownSlider, 56, 12);
	lv_obj_set_style_pad_hor(shutdownSlider, 5, 0);

	shutdownTimeLabel = lv_label_create(shutdownSlider);
	lv_obj_add_flag(shutdownTimeLabel, LV_OBJ_FLAG_FLOATING);
	lv_obj_set_style_align(shutdownTimeLabel, LV_ALIGN_CENTER, 0);
	lv_obj_set_style_text_font(shutdownTimeLabel, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_color(shutdownTimeLabel, lv_color_black(), 0);
	lv_obj_set_style_pad_top(shutdownTimeLabel, 1, 0);
	lv_obj_set_style_text_color(shutdownTimeLabel, lv_color_hex(0x892eff), 0);

	lv_obj_add_event_cb(shutdownSlider, [](lv_event_t* event){
		if(lv_obj_get_state(event->target) & LV_STATE_EDITED){
			lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}else{
			lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}
	}, LV_EVENT_STYLE_CHANGED, nullptr);

	lv_obj_add_style(shutdownSlider, &style_main, LV_PART_MAIN);
	lv_obj_add_style(shutdownSlider, &style_knob, LV_PART_KNOB);
	lv_obj_add_style(shutdownSlider, &style_knob, LV_PART_KNOB | LV_STATE_EDITED);

	//screenBrightness
	screenBrightness = lv_obj_create(flexContainer);
	lv_obj_set_height(screenBrightness, LV_SIZE_CONTENT);
	lv_obj_set_width(screenBrightness, lv_pct(100));
	lv_obj_set_layout(screenBrightness, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(screenBrightness, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(screenBrightness, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all(screenBrightness, 3, 0);
	lv_obj_set_style_bg_opa(screenBrightness, 0, 0);
	lv_obj_add_style(screenBrightness, &style_pressed, selPress);
	lv_obj_add_style(screenBrightness, &style_focused, selFocus);
	lv_obj_add_style(screenBrightness, &style_def, sel);

	lv_obj_t* brightnessLabel = lv_label_create(screenBrightness);
	lv_obj_set_style_text_font(brightnessLabel, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_color(brightnessLabel, lv_color_white(), 0);
	lv_obj_set_style_pad_top(brightnessLabel, 2, 0);
	lv_label_set_text(brightnessLabel, "Brightness");

	brightnessSlider = lv_slider_create(screenBrightness);
	lv_obj_add_flag(brightnessSlider, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	setButtonHoldAndRepeatTime(BTN_LEFT, 50);
	setButtonHoldAndRepeatTime(BTN_RIGHT, 50);
	setButtonHoldTime(BTN_LEFT, 400);
	setButtonHoldTime(BTN_RIGHT, 400);

	lv_slider_set_range(brightnessSlider, 0, 51);
	lv_obj_remove_style_all(brightnessSlider);        /*Remove the styles coming from the theme*/
	lv_obj_set_size(brightnessSlider, 56, 12);
	lv_obj_set_style_pad_hor(brightnessSlider, 5, 0);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_FOCUSED, this);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_FOCUSED);
	}, LV_EVENT_DEFOCUSED, this);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		if(!(lv_obj_get_state(event->target) & LV_STATE_EDITED)){
			slider->backToMain();
		}
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		if(lv_obj_get_state(event->target) & LV_STATE_EDITED){
			lv_obj_add_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}else{
			lv_obj_clear_state(lv_obj_get_parent(lv_event_get_target(event)), LV_STATE_EDITED);
		}
	}, LV_EVENT_STYLE_CHANGED, nullptr);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		lv_obj_t* slider = static_cast<lv_obj_t*>(event->user_data);
		lv_group_t* group = static_cast<lv_group_t*>(lv_obj_get_group(slider));
		if(group == nullptr){
			return;
		}
		lv_event_send(slider, LV_EVENT_RELEASED, nullptr);
		lv_event_send(slider, LV_EVENT_SHORT_CLICKED, nullptr);
		lv_event_send(slider, LV_EVENT_CLICKED, nullptr);
		lv_group_send_data(group, LV_KEY_ENTER);
	}, LV_EVENT_CANCEL, brightnessSlider);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		if(lv_event_get_key(event) != LV_KEY_HOME) return;
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		slider->backToMain();
	}, LV_EVENT_KEY, this);

	lv_group_add_obj(inputGroup, brightnessSlider);

	lv_obj_add_event_cb(brightnessSlider, [](lv_event_t* event){
		lv_obj_t* slider = static_cast<lv_obj_t*>(event->user_data);
		Settings.get().screenBrightness = lv_slider_get_value(slider) * 5;
		BatController.setBrightness(lv_slider_get_value(slider) * 5);
	}, LV_EVENT_VALUE_CHANGED, brightnessSlider);

	lv_obj_add_style(brightnessSlider, &style_main, LV_PART_MAIN);
	lv_obj_add_style(brightnessSlider, &style_knob, LV_PART_KNOB);
	lv_obj_add_style(brightnessSlider, &style_knob, LV_PART_KNOB | LV_STATE_EDITED);

	//pairBatmobile
	pairBatmobile = lv_obj_create(flexContainer);
	lv_obj_set_height(pairBatmobile, LV_SIZE_CONTENT);
	lv_obj_set_width(pairBatmobile, lv_pct(100));
	lv_obj_set_layout(pairBatmobile, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(pairBatmobile, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(pairBatmobile, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(pairBatmobile, 8, 0);
	lv_obj_set_style_pad_all(pairBatmobile, 3, 0);
	lv_obj_set_style_bg_opa(pairBatmobile, 0, 0);
	lv_obj_add_style(pairBatmobile, &style_focused, selFocus);
	lv_obj_add_style(pairBatmobile, &style_def, sel);
	lv_obj_add_flag(pairBatmobile, LV_OBJ_FLAG_SCROLL_ON_FOCUS);


	lv_obj_t* pairBatmobileLabel = lv_label_create(pairBatmobile);
	lv_obj_set_style_text_font(pairBatmobileLabel, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_color(pairBatmobileLabel, lv_color_white(), 0);
	lv_label_set_text(pairBatmobileLabel, "Pair another Batmobile");
	lv_obj_clear_flag(pairBatmobile, LV_OBJ_FLAG_CLICK_FOCUSABLE);
	lv_obj_clear_flag(pairBatmobile, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_clear_flag(pairBatmobile, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_add_event_cb(pairBatmobile, [](lv_event_t* event){
		auto* settings = static_cast<SettingsScreen*>(event->user_data);
		delete settings->parent;

		settings->stop();
		Com.sendDisconnectRequest();
		Com.setClient(nullptr);

		(new PairScreen())->start();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(pairBatmobile, [](lv_event_t* event){
		SettingsScreen* screen = static_cast<SettingsScreen*>(event->user_data);
		screen->backToMain();
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(pairBatmobile, [](lv_event_t* event){
		if(lv_event_get_key(event) != LV_KEY_HOME) return;
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		slider->backToMain();
	}, LV_EVENT_KEY, this);

	lv_group_add_obj(inputGroup, pairBatmobile);

	//factoryReset
	factoryReset = lv_obj_create(flexContainer);
	lv_obj_set_height(factoryReset, LV_SIZE_CONTENT);
	lv_obj_set_width(factoryReset, lv_pct(100));
	lv_obj_set_layout(factoryReset, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(factoryReset, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(factoryReset, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(factoryReset, 8, 0);
	lv_obj_set_style_pad_all(factoryReset, 3, 0);
	lv_obj_set_style_bg_opa(factoryReset, 0, 0);
	lv_obj_add_style(factoryReset, &style_focused, selFocus);
	lv_obj_add_style(factoryReset, &style_def, sel);
	lv_obj_add_flag(factoryReset, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	lv_obj_clear_flag(factoryReset, LV_OBJ_FLAG_CLICK_FOCUSABLE);
	lv_obj_clear_flag(factoryReset, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_clear_flag(factoryReset, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_add_event_cb(factoryReset, [](lv_event_t* event){
		auto hw = static_cast<SettingsScreen*>(event->user_data);
		auto& prompt = hw->factoryResetPrompt;
		lv_obj_add_event_cb(prompt.getLvObj(), [](lv_event_t* e){
			Com.sendDisconnectRequest();
			nvs_flash_erase();
			Settings.reset();
			BatController.fadeOut();
			ESP.restart();
		}, EV_PROMPT_YES, nullptr);
		prompt.start();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(factoryReset, [](lv_event_t* event){
		SettingsScreen* screen = static_cast<SettingsScreen*>(event->user_data);
		screen->backToMain();
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(factoryReset, [](lv_event_t* event){
		if(lv_event_get_key(event) != LV_KEY_HOME) return;
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		slider->backToMain();
	}, LV_EVENT_KEY, this);

	lv_group_add_obj(inputGroup, factoryReset);

	lv_obj_t* factoryResetLabel = lv_label_create(factoryReset);
	lv_obj_set_style_text_font(factoryResetLabel, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_color(factoryResetLabel, lv_color_white(), 0);
	lv_label_set_text(factoryResetLabel, "Factory reset");

	//Save and exit
	saveBtn = lv_btn_create(flexContainer);
	lv_obj_set_height(saveBtn, LV_SIZE_CONTENT);
	lv_obj_set_width(saveBtn, lv_pct(100));
	lv_obj_set_style_pad_all(saveBtn, 3, 0);
	lv_obj_set_style_bg_opa(saveBtn, 0, 0);
	lv_obj_add_style(saveBtn, &style_focused, selFocus);
	lv_obj_add_style(saveBtn, &style_def, sel);
	lv_obj_add_flag(saveBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

	lv_obj_t* saveLabel = lv_label_create(saveBtn);
	lv_obj_set_style_text_font(saveLabel, &lv_font_montserrat_10, 0);
	lv_obj_set_style_text_color(saveLabel, lv_color_white(), 0);
	lv_label_set_text(saveLabel, "Save and exit");

	lv_obj_add_event_cb(saveBtn, [](lv_event_t* event){
		auto* screen = static_cast<SettingsScreen*>(event->user_data);
		screen->backToMain();
	}, LV_EVENT_CLICKED, this);

	lv_obj_add_event_cb(saveBtn, [](lv_event_t* event){
		SettingsScreen* screen = static_cast<SettingsScreen*>(event->user_data);
		screen->backToMain();
	}, LV_EVENT_CANCEL, this);

	lv_obj_add_event_cb(saveBtn, [](lv_event_t* event){
		if(lv_event_get_key(event) != LV_KEY_HOME) return;
		SettingsScreen* slider = static_cast<SettingsScreen*>(event->user_data);
		slider->backToMain();
	}, LV_EVENT_KEY, this);

	lv_group_add_obj(inputGroup, saveBtn);
}

SettingsScreen::~SettingsScreen(){
	lv_style_reset(&style_knob);
	lv_style_reset(&style_def);
	lv_style_reset(&style_focused);
	lv_style_reset(&style_main);
	lv_style_reset(&style_pressed);
}

void SettingsScreen::onStarting(){
	lv_slider_set_value(shutdownSlider, Settings.get().shutdownTime, LV_ANIM_OFF);
	lv_slider_set_value(brightnessSlider, Settings.get().screenBrightness / 5, LV_ANIM_OFF);
	lv_slider_set_value(soundSlider, Settings.get().soundVolume / 2, LV_ANIM_OFF);

	lv_label_set_text(shutdownTimeLabel, ShutdownText[Settings.get().shutdownTime]);
}

void SettingsScreen::onStart(){
	Com.addDcListener(this);
}

void SettingsScreen::onStop(){
	factoryResetPrompt.stop();
	Settings.store();
	Com.removeDcListener(this);
}

void SettingsScreen::backToMain(){
	volatile auto parent = this->parent;

	stop();
	delete this;

	parent->start();
}

void SettingsScreen::onDisconnected(){
	stop();
	delete this;
	auto pair = new PairScreen(true);
	pair->start();
}