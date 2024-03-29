#include "ActionPickModal.h"
#include <map>

static const std::map<Simple::Action::Type, const char*> actionIcons = {
		{ Simple::Action::Type::Drive,       "S:/SimpleProg/Drive.bin" },
		{ Simple::Action::Type::Headlights,  "S:/SimpleProg/Frontlight.bin" },
		{ Simple::Action::Type::Taillights,  "S:/SimpleProg/Backlight.bin" },
		{ Simple::Action::Type::Underlights, "S:/SimpleProg/Underlight.bin" },
		{ Simple::Action::Type::Sound,       "S:/SimpleProg/Sound.bin" },
		{ Simple::Action::Type::Delay,       "S:/SimpleProg/Delay.bin" }
};

constexpr std::array<const char*, 6> ActionPickModal::actionNames;

ActionPickModal::ActionPickModal(LVScreen* parent) : LVModal(parent){
	lv_obj_set_size(obj, w, h);
	lv_obj_set_style_bg_img_src(obj, "S:/SimpleProg/ActionPickBg.bin", LV_STATE_DEFAULT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW_WRAP);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_row(obj, 5, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_column(obj, 5, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_ver(obj, 5, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_hor(obj, 5, LV_STATE_DEFAULT);

	addAction(Simple::Action::Type::Drive);
	addAction(Simple::Action::Type::Headlights);
	addAction(Simple::Action::Type::Taillights);
	addAction(Simple::Action::Type::Underlights);
	addAction(Simple::Action::Type::Sound);
	addAction(Simple::Action::Type::Delay);

	for(int j = 0; j < lv_obj_get_child_cnt(obj); ++j){
		lv_obj_add_event_cb(lv_obj_get_child(obj, j), [](lv_event_t* e){
			auto& modal = *(ActionPickModal*) e->user_data;
			auto key = lv_event_get_key(e);
			if(key == LV_KEY_UP && lv_obj_get_index(e->target) >= ActionPickModal::rowLength){
				lv_group_focus_obj(lv_obj_get_child(modal.obj, lv_obj_get_index(e->target) - ActionPickModal::rowLength));
			}else if(key == LV_KEY_DOWN && (lv_obj_get_child_cnt(modal.obj) - lv_obj_get_index(e->target) >= ActionPickModal::rowLength)){
				lv_group_focus_obj(lv_obj_get_child(modal.obj, lv_obj_get_index(e->target) + ActionPickModal::rowLength));
			}else if(key == LV_KEY_HOME || key == LV_KEY_ESC){
				modal.stop();
			}
		}, LV_EVENT_KEY, this);
	}

	actionLabel = lv_label_create(obj);
	lv_obj_set_flex_grow(actionLabel, 1);
	lv_obj_set_style_text_align(actionLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_label_set_text(actionLabel, actionNames[lv_obj_get_index(lv_group_get_focused(inputGroup))]);
	lv_obj_set_style_text_font(actionLabel, &lv_font_montserrat_10, 0);
	inputGroup->user_data = actionLabel;
	lv_obj_add_flag(actionLabel, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);

	lv_group_set_focus_cb(inputGroup, [](lv_group_t* g){
		auto actionLabel = (lv_obj_t*) g->user_data;
		lv_label_set_text(actionLabel, actionNames[lv_obj_get_index(lv_group_get_focused(g))]);

	});

}

void ActionPickModal::addAction(Simple::Action::Type type){
	lv_obj_t* actionElement = lv_img_create(obj);
	lv_img_set_src(actionElement, actionIcons.at(type));
	lv_obj_set_style_radius(actionElement, 1, LV_STATE_DEFAULT);

	lv_group_add_obj(inputGroup, actionElement);
	lv_obj_set_style_outline_color(actionElement, lv_color_white(), LV_STATE_FOCUSED);
	lv_obj_set_style_outline_opa(actionElement, LV_OPA_COVER, LV_STATE_FOCUSED);
	lv_obj_set_style_outline_width(actionElement, 2, LV_STATE_FOCUSED);

	lv_obj_add_event_cb(actionElement, [](lv_event_t* e){
		auto index = lv_obj_get_index(e->target);
		auto& modal = *(ActionPickModal*) e->user_data;
		auto type = (Simple::Action::Type) index;

		Simple::Action action;
		action.type = type;
		switch(type){
			case Simple::Action::Type::Drive:
				action.type = Simple::Action::Type::Drive;
				action.DriveData = { DriveDirection::Forward, 10, 100 };
				break;
			case Simple::Action::Type::Headlights:
				action.HeadTaillightData.toggle = true;
				break;
			case Simple::Action::Type::Taillights:
				action.HeadTaillightData.toggle = true;
				break;
			case Simple::Action::Type::Underlights:
				action.RGBData.color = Simple::Red;
				break;
			case Simple::Action::Type::Sound:
				action.SoundData.sampleIndex = 0;
				break;
			case Simple::Action::Type::Delay:
				action.DelayData.duration = 10;
				break;
		}
		if(modal.pickedCallback) modal.pickedCallback(action);
		modal.stop();
	}, LV_EVENT_PRESSED, this);
}

void ActionPickModal::onStop(){
	pickedCallback = {};
}

void ActionPickModal::onStart(){
	lv_group_focus_obj(lv_obj_get_child(obj, 0));
}

void ActionPickModal::startPick(std::function<void(Simple::Action)> pickedCallback){
	this->pickedCallback = pickedCallback;
	start();
}
