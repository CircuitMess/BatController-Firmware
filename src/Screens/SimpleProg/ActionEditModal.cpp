#include "ActionEditModal.h"
#include <BatController.h>
#include "../../InputLVGL.h"

ActionEditModal::ActionEditModal(LVScreen* parent) : LVModal(parent){

	lv_obj_set_size(obj, w, h);
	lv_obj_set_style_bg_img_src(obj, "S:/DriveScreen/ModalBg.bin", LV_STATE_DEFAULT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_row(obj, 1, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_all(obj, 5, LV_STATE_DEFAULT);


}

void ActionEditModal::startEdit(Simple::Action& action){
	for(int j = lv_obj_get_child_cnt(obj) - 1; j >= 0; --j){
		lv_obj_del(lv_obj_get_child(obj, j));
	}


	if(action.type == Simple::Action::Type::Drive){
		addDirection(action);
		addDuration(action);
		addSpeed(action);
	}else if(action.type == Simple::Action::Type::Delay){
		addDuration(action);
	}else if(action.type == Simple::Action::Type::Headlights || action.type == Simple::Action::Type::Taillights){
		addToggle(action);
	}else if(action.type == Simple::Action::Type::Sound){
		addSound(action);
		Com.sendSoundEffect(action.SoundData.sampleIndex);
	}else if(action.type == Simple::Action::Type::Underlights){
		addColor(action);
	}

	for(int j = 0; j < lv_obj_get_child_cnt(obj); ++j){
		lv_obj_add_event_cb(lv_obj_get_child(obj, j), [](lv_event_t* e){
			auto key = lv_event_get_key(e);
			if(key == LV_KEY_HOME || key == LV_KEY_ESC){
				auto modal = (ActionEditModal*) e->user_data;
				modal->stop();
			}
		}, LV_EVENT_KEY, this);
	}

	start();
}

void ActionEditModal::onStart(){
	lv_group_focus_obj(lv_obj_get_child(obj, 0));
	InputLVGL::enableHorizontalNavigation(false);
	InputLVGL::enableVerticalNavigation(true);
}

void ActionEditModal::onStop(){
	InputLVGL::enableHorizontalNavigation(true);
	InputLVGL::enableVerticalNavigation(false);
}

void ActionEditModal::addDirection(Simple::Action& action){
	lv_obj_t* dir = lv_obj_create(obj);
	lv_obj_set_size(dir, entryW, entryH);
	lv_obj_set_style_pad_all(dir, 1, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(dir, 4, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(dir, 4, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(dir, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_src(dir,"S:/SimpleProg/Frame.bin", LV_STATE_FOCUSED);
	lv_obj_set_style_bg_img_opa(dir, LV_OPA_COVER, LV_STATE_FOCUSED);
	lv_obj_t* label = lv_label_create(dir);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(label, "Direction");
	lv_obj_set_align(label, LV_ALIGN_LEFT_MID);

	lv_obj_t* indicator = lv_label_create(dir);
	lv_obj_set_style_text_font(indicator, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(indicator, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(indicator, DirectionSymbols[(uint8_t) action.DriveData.dir]);
	lv_obj_set_align(indicator, LV_ALIGN_RIGHT_MID);

	lv_obj_add_event_cb(dir, [](lv_event_t* e){
		auto& action = *(Simple::Action*) e->user_data;
		auto indicator = lv_obj_get_child(e->target, 1);

		if(lv_event_get_key(e) == LV_KEY_LEFT){
			action.DriveData.dir =
					(action.DriveData.dir == DriveDirection::Forward) ? DriveDirection::Right : (DriveDirection) ((((uint8_t) action.DriveData.dir) - 1) % 4);
			lv_label_set_text(indicator, DirectionSymbols[(uint8_t) action.DriveData.dir]);
		}else if(lv_event_get_key(e) == LV_KEY_RIGHT){
			action.DriveData.dir = (DriveDirection) ((((uint8_t) action.DriveData.dir) + 1) % 4);
			lv_label_set_text(indicator, DirectionSymbols[(uint8_t) action.DriveData.dir]);
		}
	}, LV_EVENT_KEY, &action);

	lv_group_add_obj(inputGroup, dir);
}

void ActionEditModal::addDuration(Simple::Action& action){
	lv_obj_t* dir = lv_obj_create(obj);
	lv_obj_set_size(dir, entryW, entryH);
	lv_obj_set_style_pad_all(dir, 1, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(dir, 4, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(dir, 4, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(dir, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	if(action.type == Simple::Action::Type::Drive){
		lv_obj_set_style_bg_img_src(dir,"S:/SimpleProg/Frame.bin", LV_STATE_FOCUSED);
		lv_obj_set_style_bg_img_opa(dir, LV_OPA_COVER, LV_STATE_FOCUSED);
	}
	lv_obj_t* label = lv_label_create(dir);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(label, "Duration");
	lv_obj_set_align(label, LV_ALIGN_LEFT_MID);

	lv_obj_t* indicator = lv_label_create(dir);
	lv_obj_set_style_text_font(indicator, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(indicator, lv_color_black(), LV_STATE_DEFAULT);
	auto duration = (action.type == Simple::Action::Type::Drive ? action.DriveData.duration : action.DelayData.duration);
	lv_label_set_text_fmt(indicator, "%d.%ds", duration / 10, duration % 10);
	lv_obj_set_align(indicator, LV_ALIGN_RIGHT_MID);

	lv_obj_add_event_cb(dir, [](lv_event_t* e){
		auto& action = *(Simple::Action*) e->user_data;
		auto indicator = lv_obj_get_child(e->target, 1);
		auto& duration = (action.type == Simple::Action::Type::Drive ? action.DriveData.duration : action.DelayData.duration);

		if(lv_event_get_key(e) == LV_KEY_LEFT){
			duration = constrain(duration - 1, 0, 99);
			lv_label_set_text_fmt(indicator, "%d.%ds", duration / 10, duration % 10);
		}else if(lv_event_get_key(e) == LV_KEY_RIGHT){
			duration = constrain(duration + 1, 0, 99);
			lv_label_set_text_fmt(indicator, "%d.%ds", duration / 10, duration % 10);
		}
	}, LV_EVENT_KEY, &action);

	lv_group_add_obj(inputGroup, dir);
}

void ActionEditModal::addSpeed(Simple::Action& action){
	lv_obj_t* dir = lv_obj_create(obj);
	lv_obj_set_size(dir, entryW, entryH);
	lv_obj_set_style_pad_all(dir, 1, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(dir, 4, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(dir, 4, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(dir, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_src(dir,"S:/SimpleProg/Frame.bin", LV_STATE_FOCUSED);
	lv_obj_set_style_bg_img_opa(dir, LV_OPA_COVER, LV_STATE_FOCUSED);
	lv_obj_t* label = lv_label_create(dir);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(label, "Speed");
	lv_obj_set_align(label, LV_ALIGN_LEFT_MID);

	lv_obj_t* indicator = lv_label_create(dir);
	lv_obj_set_style_text_font(indicator, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(indicator, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(indicator, "%d", action.DriveData.speed);
	lv_obj_set_align(indicator, LV_ALIGN_RIGHT_MID);

	lv_obj_add_event_cb(dir, [](lv_event_t* e){
		auto& action = *(Simple::Action*) e->user_data;
		auto indicator = lv_obj_get_child(e->target, 1);

		if(lv_event_get_key(e) == LV_KEY_LEFT){
			action.DriveData.speed = constrain(action.DriveData.speed - 1, 0, 100);
			lv_label_set_text_fmt(indicator, "%d", action.DriveData.speed);
		}else if(lv_event_get_key(e) == LV_KEY_RIGHT){
			action.DriveData.speed = constrain(action.DriveData.speed + 1, 0, 100);
			lv_label_set_text_fmt(indicator, "%d", action.DriveData.speed);
		}
	}, LV_EVENT_KEY, &action);

	lv_group_add_obj(inputGroup, dir);
}

void ActionEditModal::addToggle(Simple::Action& action){
	lv_obj_t* dir = lv_obj_create(obj);
	lv_obj_set_size(dir, entryW, entryH * 2);
	lv_obj_set_style_pad_all(dir, 1, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(dir, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	lv_obj_t* label = lv_label_create(dir);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
	if(action.type == Simple::Action::Type::Headlights){
		lv_label_set_text(label, "Headlights");
	}else if(action.type == Simple::Action::Type::Taillights){
		lv_label_set_text(label, "Taillights");
	}
	lv_obj_set_align(label, LV_ALIGN_TOP_MID);

	lv_obj_t* indicator = lv_label_create(dir);
	lv_obj_set_style_text_font(indicator, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(indicator, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(indicator, action.HeadTaillightData.toggle ? "ON" : "OFF");
	lv_obj_set_align(indicator, LV_ALIGN_BOTTOM_MID);

	lv_obj_add_event_cb(dir, [](lv_event_t* e){
		auto& action = *(Simple::Action*) e->user_data;
		auto indicator = lv_obj_get_child(e->target, 1);

		if(lv_event_get_key(e) == LV_KEY_LEFT){
			action.HeadTaillightData.toggle = !action.HeadTaillightData.toggle;
			lv_label_set_text(indicator, action.HeadTaillightData.toggle ? "ON" : "OFF");
		}else if(lv_event_get_key(e) == LV_KEY_RIGHT){
			action.HeadTaillightData.toggle = !action.HeadTaillightData.toggle;
			lv_label_set_text(indicator, action.HeadTaillightData.toggle ? "ON" : "OFF");
		}
	}, LV_EVENT_KEY, &action);

	lv_group_add_obj(inputGroup, dir);
}

void ActionEditModal::addSound(Simple::Action& action){
	lv_obj_t* dir = lv_obj_create(obj);
	lv_obj_set_size(dir, entryW, entryH * 2);
	lv_obj_set_style_pad_all(dir, 1, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(dir, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	lv_obj_t* label = lv_label_create(dir);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(label, "Sound");
	lv_obj_set_align(label, LV_ALIGN_TOP_MID);

	lv_obj_t* indicator = lv_label_create(dir);
	lv_obj_set_style_text_font(indicator, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(indicator, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(indicator, SoundNames[action.SoundData.sampleIndex]);
	lv_obj_set_align(indicator, LV_ALIGN_BOTTOM_MID);

	lv_obj_add_event_cb(dir, [](lv_event_t* e){
		auto& action = *(Simple::Action*) e->user_data;
		auto indicator = lv_obj_get_child(e->target, 1);

		if(lv_event_get_key(e) == LV_KEY_LEFT){
			action.SoundData.sampleIndex = constrain(action.SoundData.sampleIndex - 1, 0, SoundsNum - 1);
			lv_label_set_text(indicator, SoundNames[action.SoundData.sampleIndex]);
			Com.sendSoundEffect(action.SoundData.sampleIndex);
		}else if(lv_event_get_key(e) == LV_KEY_RIGHT){
			action.SoundData.sampleIndex = constrain(action.SoundData.sampleIndex + 1, 0, SoundsNum - 1);
			lv_label_set_text(indicator, SoundNames[action.SoundData.sampleIndex]);
			Com.sendSoundEffect(action.SoundData.sampleIndex);
		}
	}, LV_EVENT_KEY, &action);

	lv_group_add_obj(inputGroup, dir);
}

void ActionEditModal::addColor(Simple::Action& action){
	lv_obj_t* dir = lv_obj_create(obj);
	lv_obj_set_size(dir, entryW, entryH * 2);
	lv_obj_set_style_pad_all(dir, 1, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(dir, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	lv_obj_t* label = lv_label_create(dir);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(label, "Underlights");
	lv_obj_set_align(label, LV_ALIGN_TOP_MID);

	lv_obj_t* indicator = lv_label_create(dir);
	lv_obj_set_style_text_font(indicator, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(indicator, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(indicator, ColorNames[action.RGBData.color]);
	lv_obj_set_align(indicator, LV_ALIGN_BOTTOM_MID);

	lv_obj_add_event_cb(dir, [](lv_event_t* e){
		auto& action = *(Simple::Action*) e->user_data;
		auto indicator = lv_obj_get_child(e->target, 1);

		if(lv_event_get_key(e) == LV_KEY_LEFT){
			action.RGBData.color = (Simple::Color) constrain(action.RGBData.color - 1, 0, ColorsNum - 1);
			lv_label_set_text(indicator, ColorNames[action.RGBData.color]);
		}else if(lv_event_get_key(e) == LV_KEY_RIGHT){
			action.RGBData.color = (Simple::Color) constrain(action.RGBData.color + 1, 0, ColorsNum - 1);
			lv_label_set_text(indicator, ColorNames[action.RGBData.color]);
		}
	}, LV_EVENT_KEY, &action);

	lv_group_add_obj(inputGroup, dir);
}
