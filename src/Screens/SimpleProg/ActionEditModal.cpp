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
	editingAction = &action;

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
	lv_anim_del(arrowLeft, nullptr);
	lv_anim_del(arrowRight, nullptr);
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
	lv_label_set_text(label, action.type == Simple::Action::Type::Delay ? "Delay" : "Duration");
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

	lv_obj_t* indicator = lv_obj_create(dir);
	lv_obj_set_scrollbar_mode(indicator, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_size(indicator, entryW, entryH);
	lv_obj_set_align(indicator, LV_ALIGN_BOTTOM_MID);
	lv_obj_set_layout(indicator, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(indicator, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(indicator, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(indicator, 5, LV_STATE_DEFAULT);

	arrowLeft = lv_img_create(indicator);

	lv_obj_t* text = lv_label_create(indicator);
	lv_obj_set_style_text_font(text, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(text, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
	lv_label_set_text(text, action.HeadTaillightData.toggle ? "ON" : "OFF");
	lv_label_set_long_mode(text, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_set_width(text, 40);

	arrowRight = lv_img_create(indicator);
	lv_img_set_src(arrowLeft, LV_SYMBOL_LEFT);
	lv_img_set_src(arrowRight, LV_SYMBOL_RIGHT);
	lv_obj_set_style_opa(arrowLeft, LV_OPA_COVER, 0);
	lv_obj_set_style_opa(arrowRight, LV_OPA_COVER, 0);
	lv_obj_set_style_text_font(arrowLeft, &lv_font_montserrat_8, 0);
	lv_obj_set_style_text_font(arrowRight, &lv_font_montserrat_8, 0);
	lv_obj_set_style_text_color(arrowLeft, lv_color_black(), 0);
	lv_obj_set_style_text_color(arrowRight, lv_color_black(), 0);

	lv_anim_init(&anim);
	lv_anim_set_var(&anim, arrowLeft);
	lv_anim_set_playback_time(&anim, 75);
	lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&anim, lv_anim_path_step);
	lv_anim_set_exec_cb(&anim, [](void* obj, int32_t v){
		lv_obj_set_style_translate_x((lv_obj_t*) obj, v, 0);
		lv_obj_invalidate(((lv_obj_t*) obj)->parent);
	});
	lv_anim_set_values(&anim, -2, 0);
	lv_anim_set_values(&anim, 2, 0);
	lv_anim_set_var(&anim, arrowRight);

	lv_obj_add_event_cb(dir, [](lv_event_t* e){
		auto& modal = *(ActionEditModal*) e->user_data;
		auto& action = *modal.editingAction;
		auto indicator = lv_obj_get_child(e->target, 1);
		auto txt = lv_obj_get_child(indicator, 1);

		if(lv_event_get_key(e) == LV_KEY_LEFT && action.HeadTaillightData.toggle){
			Serial.println("left");
			action.HeadTaillightData.toggle = false;
			lv_label_set_text(txt, action.HeadTaillightData.toggle ? "ON" : "OFF");

			lv_anim_del(modal.arrowLeft, nullptr);
			lv_anim_del(modal.arrowRight, nullptr);

			lv_anim_set_values(&modal.anim, 2, 0);
			lv_anim_set_var(&modal.anim, modal.arrowRight);
			lv_anim_start(&modal.anim);

		}else if(lv_event_get_key(e) == LV_KEY_RIGHT && !action.HeadTaillightData.toggle){
			Serial.println("right");
			action.HeadTaillightData.toggle = true;
			lv_label_set_text(txt, action.HeadTaillightData.toggle ? "ON" : "OFF");

			lv_anim_del(modal.arrowLeft, nullptr);
			lv_anim_del(modal.arrowRight, nullptr);

			lv_anim_set_values(&modal.anim, -2, 0);
			lv_anim_set_var(&modal.anim, modal.arrowLeft);
			lv_anim_start(&modal.anim);
		}
	}, LV_EVENT_KEY, this);

	if(action.HeadTaillightData.toggle){
		lv_anim_set_values(&anim, -2, 0);
		lv_anim_set_var(&anim, arrowLeft);
		lv_anim_start(&anim);
	}else{
		lv_anim_set_values(&anim, 2, 0);
		lv_anim_set_var(&anim, arrowRight);
		lv_anim_start(&anim);
	}

	lv_group_add_obj(inputGroup, dir);
}

void ActionEditModal::addSound(Simple::Action& action){
	lv_obj_t* dir = lv_obj_create(obj);
	lv_obj_set_size(dir, entryW, entryH * 2);
	lv_obj_set_style_bg_opa(dir, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	lv_obj_t* label = lv_label_create(dir);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(label, "Sound");
	lv_obj_set_align(label, LV_ALIGN_TOP_MID);

	lv_obj_t* indicator = lv_obj_create(dir);
	lv_obj_set_scrollbar_mode(indicator, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_size(indicator, entryW, entryH);
	lv_obj_set_align(indicator, LV_ALIGN_BOTTOM_MID);
	lv_obj_set_layout(indicator, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(indicator, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(indicator, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(indicator, 2, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_hor(indicator, 3, LV_STATE_DEFAULT);


	arrowLeft = lv_img_create(indicator);

	lv_obj_t* text = lv_label_create(indicator);
	lv_obj_set_style_text_font(text, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(text, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
	lv_label_set_text(text, SoundNames[action.SoundData.sampleIndex]);
	lv_label_set_long_mode(text, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_set_flex_grow(text, 1);

	arrowRight = lv_img_create(indicator);
	lv_img_set_src(arrowLeft, LV_SYMBOL_LEFT);
	lv_img_set_src(arrowRight, LV_SYMBOL_RIGHT);
	lv_obj_set_style_opa(arrowLeft, LV_OPA_COVER, 0);
	lv_obj_set_style_opa(arrowRight, LV_OPA_COVER, 0);
	lv_obj_set_style_text_font(arrowLeft, &lv_font_montserrat_8, 0);
	lv_obj_set_style_text_font(arrowRight, &lv_font_montserrat_8, 0);
	lv_obj_set_style_text_color(arrowLeft, lv_color_black(), 0);
	lv_obj_set_style_text_color(arrowRight, lv_color_black(), 0);

	lv_anim_init(&anim);
	lv_anim_set_var(&anim, arrowLeft);
	lv_anim_set_playback_time(&anim, 75);
	lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&anim, lv_anim_path_step);
	lv_anim_set_exec_cb(&anim, [](void* obj, int32_t v){
		lv_obj_set_style_translate_x((lv_obj_t*) obj, v, 0);
		lv_obj_invalidate(((lv_obj_t*) obj)->parent);
	});
	lv_anim_set_values(&anim, -2, 0);
	lv_anim_set_values(&anim, 2, 0);
	lv_anim_set_var(&anim, arrowRight);

	lv_obj_add_event_cb(dir, [](lv_event_t* e){
		auto& modal = *(ActionEditModal*) e->user_data;
		auto& action = *modal.editingAction;
		auto indicator = lv_obj_get_child(e->target, 1);
		auto txt = lv_obj_get_child(indicator, 1);

		if(lv_event_get_key(e) == LV_KEY_LEFT){
			action.SoundData.sampleIndex = constrain(action.SoundData.sampleIndex - 1, 0, SoundsNum - 1);
			lv_label_set_text(txt, SoundNames[action.SoundData.sampleIndex]);

			lv_anim_del(modal.arrowLeft, nullptr);
			lv_anim_del(modal.arrowRight, nullptr);

			if(action.SoundData.sampleIndex != 0){
				lv_anim_set_values(&modal.anim, -2, 0);
				lv_anim_set_var(&modal.anim, modal.arrowLeft);
				lv_anim_start(&modal.anim);
			}

			lv_anim_set_values(&modal.anim, 2, 0);
			lv_anim_set_var(&modal.anim, modal.arrowRight);
			lv_anim_start(&modal.anim);

			Com.sendSoundEffect(action.SoundData.sampleIndex);
		}else if(lv_event_get_key(e) == LV_KEY_RIGHT){
			action.SoundData.sampleIndex = constrain(action.SoundData.sampleIndex + 1, 0, SoundsNum - 1);
			lv_label_set_text(txt, SoundNames[action.SoundData.sampleIndex]);

			lv_anim_del(modal.arrowLeft, nullptr);
			lv_anim_del(modal.arrowRight, nullptr);

			lv_anim_set_values(&modal.anim, -2, 0);
			lv_anim_set_var(&modal.anim, modal.arrowLeft);
			lv_anim_start(&modal.anim);

			if(action.SoundData.sampleIndex != SoundsNum - 1){
				lv_anim_set_values(&modal.anim, 2, 0);
				lv_anim_set_var(&modal.anim, modal.arrowRight);
				lv_anim_start(&modal.anim);
			}
			Com.sendSoundEffect(action.SoundData.sampleIndex);
		}
	}, LV_EVENT_KEY, this);

	if(action.SoundData.sampleIndex > 0){
		lv_anim_set_values(&anim, -2, 0);
		lv_anim_set_var(&anim, arrowLeft);
		lv_anim_start(&anim);
	}
	if(action.SoundData.sampleIndex < (SoundsNum - 1)){
		lv_anim_set_values(&anim, 2, 0);
		lv_anim_set_var(&anim, arrowRight);
		lv_anim_start(&anim);
	}

	lv_group_add_obj(inputGroup, dir);
}

void ActionEditModal::addColor(Simple::Action& action){
	lv_obj_t* dir = lv_obj_create(obj);
	lv_obj_set_size(dir, entryW, entryH * 2);
	lv_obj_set_style_bg_opa(dir, LV_OPA_TRANSP, LV_STATE_DEFAULT);
	lv_obj_t* label = lv_label_create(dir);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text(label, "Underlights");
	lv_obj_set_align(label, LV_ALIGN_TOP_MID);

	lv_obj_t* indicator = lv_obj_create(dir);
	lv_obj_set_scrollbar_mode(indicator, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_size(indicator, entryW, entryH);
	lv_obj_set_align(indicator, LV_ALIGN_BOTTOM_MID);
	lv_obj_set_layout(indicator, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(indicator, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(indicator, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_gap(indicator, 2, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_hor(indicator, 3, LV_STATE_DEFAULT);


	arrowLeft = lv_img_create(indicator);

	lv_obj_t* text = lv_label_create(indicator);
	lv_obj_set_style_text_font(text, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(text, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
	lv_label_set_text(text, ColorNames[action.RGBData.color]);
	lv_label_set_long_mode(text, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_set_flex_grow(text, 1);

	arrowRight = lv_img_create(indicator);
	lv_img_set_src(arrowLeft, LV_SYMBOL_LEFT);
	lv_img_set_src(arrowRight, LV_SYMBOL_RIGHT);
	lv_obj_set_style_opa(arrowLeft, LV_OPA_COVER, 0);
	lv_obj_set_style_opa(arrowRight, LV_OPA_COVER, 0);
	lv_obj_set_style_text_font(arrowLeft, &lv_font_montserrat_8, 0);
	lv_obj_set_style_text_font(arrowRight, &lv_font_montserrat_8, 0);
	lv_obj_set_style_text_color(arrowLeft, lv_color_black(), 0);
	lv_obj_set_style_text_color(arrowRight, lv_color_black(), 0);

	lv_anim_init(&anim);
	lv_anim_set_var(&anim, arrowLeft);
	lv_anim_set_playback_time(&anim, 75);
	lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
	lv_anim_set_path_cb(&anim, lv_anim_path_step);
	lv_anim_set_exec_cb(&anim, [](void* obj, int32_t v){
		lv_obj_set_style_translate_x((lv_obj_t*) obj, v, 0);
		lv_obj_invalidate(((lv_obj_t*) obj)->parent);
	});
	lv_anim_set_values(&anim, -2, 0);
	lv_anim_set_values(&anim, 2, 0);
	lv_anim_set_var(&anim, arrowRight);

	lv_obj_add_event_cb(dir, [](lv_event_t* e){
		auto& modal = *(ActionEditModal*) e->user_data;
		auto& action = *modal.editingAction;
		auto indicator = lv_obj_get_child(e->target, 1);
		auto txt = lv_obj_get_child(indicator, 1);

		if(lv_event_get_key(e) == LV_KEY_LEFT){
			action.RGBData.color = (Simple::Color) constrain(action.RGBData.color - 1, 0, ColorsNum - 1);
			lv_label_set_text(txt, ColorNames[action.RGBData.color]);

			lv_anim_del(modal.arrowLeft, nullptr);
			lv_anim_del(modal.arrowRight, nullptr);

			if(action.RGBData.color != 0){
				lv_anim_set_values(&modal.anim, -2, 0);
				lv_anim_set_var(&modal.anim, modal.arrowLeft);
				lv_anim_start(&modal.anim);
			}

			lv_anim_set_values(&modal.anim, 2, 0);
			lv_anim_set_var(&modal.anim, modal.arrowRight);
			lv_anim_start(&modal.anim);

		}else if(lv_event_get_key(e) == LV_KEY_RIGHT){
			action.RGBData.color = (Simple::Color) constrain(action.RGBData.color + 1, 0, ColorsNum - 1);
			lv_label_set_text(txt, ColorNames[action.RGBData.color]);

			lv_anim_del(modal.arrowLeft, nullptr);
			lv_anim_del(modal.arrowRight, nullptr);

			lv_anim_set_values(&modal.anim, -2, 0);
			lv_anim_set_var(&modal.anim, modal.arrowLeft);
			lv_anim_start(&modal.anim);

			if(action.RGBData.color != ColorsNum - 1){
				lv_anim_set_values(&modal.anim, 2, 0);
				lv_anim_set_var(&modal.anim, modal.arrowRight);
				lv_anim_start(&modal.anim);
			}
		}
	}, LV_EVENT_KEY, this);

	if(action.RGBData.color > 0){
		lv_anim_set_values(&anim, -2, 0);
		lv_anim_set_var(&anim, arrowLeft);
		lv_anim_start(&anim);
	}
	if(action.RGBData.color < (ColorsNum - 1)){
		lv_anim_set_values(&anim, 2, 0);
		lv_anim_set_var(&anim, arrowRight);
		lv_anim_start(&anim);
	}
	lv_group_add_obj(inputGroup, dir);
}
