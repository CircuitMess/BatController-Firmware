#include "ProgPlaybackElement.h"

ProgPlaybackElement::ProgPlaybackElement(lv_obj_t* parent, const Simple::Program& program) : LVObject(parent), program(program){

	lv_obj_set_size(obj, 150, 110);
	lv_obj_set_pos(obj, 10, 18);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_CENTER);
	lv_obj_set_style_pad_gap(obj, 2, 0);
	lv_obj_set_style_pad_bottom(obj, 10, 0);

	lv_style_init(&border);
	lv_style_set_radius(&border, 1);
	lv_style_set_outline_color(&border, lv_palette_main(LV_PALETTE_RED));
	lv_style_set_outline_opa(&border, LV_OPA_COVER);
	lv_style_set_outline_width(&border, 2);

	for(auto action: program.actions){
		lv_obj_t* prObj = lv_obj_create(obj);
		lv_obj_set_size(prObj, 150, 19);

		lv_obj_t* img = lv_img_create(prObj);
		lv_obj_align(img, LV_ALIGN_LEFT_MID, 2, 0);

		lv_obj_t* text = lv_label_create(prObj);
		lv_obj_set_style_text_color(text, lv_color_white(), 0);
		lv_obj_set_style_text_font(text, &lv_font_montserrat_10, 0);
		lv_obj_set_align(text, LV_ALIGN_CENTER);
		lv_obj_set_style_bg_color(text, lv_color_black(), LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(text, LV_OPA_60, LV_STATE_DEFAULT);

		switch(action.type){
			case Simple::Action::Type::Drive:
				lv_img_set_src(img, "S:/SimpleProg/Drive.bin");
				lv_label_set_text(text, (String("Driving ") + DirectionSymbols[(uint8_t) action.DriveData.dir]
										 + " " + (float) action.DriveData.duration / 10 + "s").c_str());
				break;
			case Simple::Action::Type::Headlights:
				lv_img_set_src(img, "S:/SimpleProg/Frontlight.bin");
				lv_label_set_text(text, (String("Headlights ") + (action.HeadTaillightData.toggle ? "ON" : "OFF")).c_str());
				break;
			case Simple::Action::Type::Taillights:
				lv_img_set_src(img, "S:/SimpleProg/Backlight.bin");
				lv_label_set_text(text, (String("Taillights ") + (action.HeadTaillightData.toggle ? "ON" : "OFF")).c_str());
				break;
			case Simple::Action::Type::Underlights:
				lv_img_set_src(img, "S:/SimpleProg/Underlight.bin");
				lv_label_set_text(text, (String("Under-lights ") + ColorNames[action.RGBData.color]).c_str());
				break;
			case Simple::Action::Type::Sound:
				lv_img_set_src(img, "S:/SimpleProg/Sound.bin");
				lv_label_set_text(text, (String("Play sound ") + SoundNames[action.SoundData.sampleIndex]).c_str());
				lv_label_set_long_mode(text, LV_LABEL_LONG_SCROLL_CIRCULAR);
				break;
			case Simple::Action::Type::Delay:
				lv_img_set_src(img, "S:/SimpleProg/Delay.bin");
				lv_label_set_text(text, (String("Delay ") + (float) action.DelayData.duration / 10 + "s").c_str());
				break;
		}

	}

	doneElement = lv_obj_create(obj);
	lv_obj_set_size(doneElement, 150, 38);

	doneLabel = lv_label_create(doneElement);
	lv_obj_set_style_text_color(doneLabel, lv_color_white(), 0);
	lv_obj_set_style_text_font(doneLabel, &lv_font_montserrat_10, 0);
	lv_obj_set_style_bg_color(doneLabel, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(doneLabel, LV_OPA_60, LV_STATE_DEFAULT);
	lv_label_set_text(doneLabel, "Done!\nPress MENU to return");
	lv_obj_set_align(doneLabel, LV_ALIGN_CENTER);
	lv_obj_set_style_text_align(doneLabel, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);


	if(program.actions.empty()){
		lv_obj_set_style_text_color(doneLabel, lv_color_make(255, 0, 0), 0);
	}else{
		lv_obj_t* tmp = lv_obj_get_child(obj, 0);
		lv_obj_scroll_to(obj, 0, 0 * 20, LV_ANIM_ON);
		lv_obj_set_style_text_color(lv_obj_get_child(tmp, 1), lv_color_make(255, 0, 0), 0);
		lv_obj_add_style(lv_obj_get_child(tmp, 0), &border, 0);
	}
}

void ProgPlaybackElement::nextAction(){
	if(actionCursor > program.actions.size() - 1) return;

	lv_obj_t* tmp = lv_obj_get_child(obj, actionCursor);
	lv_obj_set_style_text_color(lv_obj_get_child(tmp, 1), lv_color_white(), 0);
	lv_obj_remove_style(lv_obj_get_child(tmp, 0), &border, 0);

	if(actionCursor == program.actions.size() - 1){
		lv_obj_scroll_to(obj, 0, actionCursor * 20, LV_ANIM_ON);
		lv_obj_set_style_text_color(doneLabel, lv_color_make(255, 0, 0), 0);
	}else{
		actionCursor++;
		lv_obj_scroll_to(obj, 0, actionCursor * 20, LV_ANIM_ON);
		tmp = lv_obj_get_child(obj, actionCursor);
		lv_obj_set_style_text_color(lv_obj_get_child(tmp, 1), lv_color_make(255, 0, 0), 0);
		lv_obj_add_style(lv_obj_get_child(tmp, 0), &border, 0);
	}
}

ProgPlaybackElement::~ProgPlaybackElement(){
	lv_style_reset(&border);
}
