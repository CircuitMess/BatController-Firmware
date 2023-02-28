#include "ProgPlaybackElement.h"

ProgPlaybackElement::ProgPlaybackElement(lv_obj_t* parent, const Simple::Program& program) : LVObject(parent), program(program){

	lv_obj_set_size(obj, 160, 110);
	lv_obj_set_pos(obj, 0, 18);
	lv_obj_set_style_pad_hor(obj, 10, LV_STATE_DEFAULT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_CENTER);
	lv_obj_set_style_pad_gap(obj, 10, 0);
	lv_obj_set_style_pad_bottom(obj, 10, 0);

	lv_style_init(&border);
	lv_style_set_radius(&border, 0);
	lv_style_set_border_color(&border, lv_palette_main(LV_PALETTE_RED));
	lv_style_set_border_opa(&border, LV_OPA_COVER);
	lv_style_set_border_width(&border, 1);
	lv_style_set_radius(&border, 3);
	lv_style_set_bg_color(&border, lv_palette_main(LV_PALETTE_RED));
	lv_style_set_bg_opa(&border, LV_OPA_COVER);
	lv_style_set_pad_all(&border, 0);

	lv_style_init(&noBorder);
	lv_style_set_pad_all(&noBorder, 1);

	for(auto action: program.actions){
		lv_obj_t* prObj = lv_obj_create(obj);
		lv_obj_set_size(prObj, 160 - lv_obj_get_style_pad_left(obj, LV_PART_MAIN) - lv_obj_get_style_pad_right(obj, LV_PART_MAIN), LV_SIZE_CONTENT);
		lv_obj_set_layout(prObj, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(prObj, LV_FLEX_FLOW_ROW);
		lv_obj_set_flex_align(prObj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
		lv_obj_set_style_pad_gap(prObj, 5, LV_STATE_DEFAULT);

		lv_obj_t* img = lv_img_create(prObj);
		lv_obj_add_style(img, &noBorder, 0);

		lv_obj_t* textContainer = lv_obj_create(prObj);
		lv_obj_set_flex_grow(textContainer, 1);

		lv_obj_t* text = lv_label_create(textContainer);
		lv_obj_set_style_text_color(text, lv_color_white(), 0);
		lv_obj_set_style_text_font(text, &lv_font_montserrat_10, 0);
		lv_obj_set_align(text, LV_ALIGN_CENTER);
		lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(text, lv_color_black(), LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(text, LV_OPA_60, LV_STATE_DEFAULT);
		lv_obj_set_style_pad_hor(text, 2, LV_STATE_DEFAULT);

		switch(action.type){
			case Simple::Action::Type::Drive:
				lv_img_set_src(img, "S:/SimpleProg/Drive.bin");
				lv_label_set_text(text, (String("Driving \n") + DirectionSymbols[(uint8_t) action.DriveData.dir]
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
		lv_obj_update_layout(textContainer);
		lv_obj_set_height(textContainer, LV_SIZE_CONTENT);
		lv_obj_update_layout(prObj);
		lv_obj_set_height(prObj, LV_SIZE_CONTENT);
		if(lv_obj_get_width(text) > lv_obj_get_width(textContainer)){
			lv_obj_set_width(text, lv_obj_get_width(textContainer));
			lv_label_set_long_mode(text, LV_LABEL_LONG_WRAP);
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
	lv_obj_set_style_pad_hor(doneLabel, 2, LV_STATE_DEFAULT);

	lv_obj_set_size(obj, 160, 110);
	lv_obj_set_pos(obj, 0, 18);

	if(program.actions.empty()){
		lv_obj_set_style_text_color(doneLabel, lv_color_make(255, 0, 0), 0);
	}else{
		lv_obj_t* prObj = lv_obj_get_child(obj, 0);
		lv_obj_set_style_text_color(lv_obj_get_child(lv_obj_get_child(prObj, 1), 0), lv_color_make(255, 0, 0), 0);
		lv_obj_remove_style(lv_obj_get_child(prObj, 0), &noBorder, 0);
		lv_obj_add_style(lv_obj_get_child(prObj, 0), &border, 0);
	}

	lv_obj_update_layout(obj);
}

void ProgPlaybackElement::nextAction(){
	if(actionCursor > program.actions.size() - 1) return;

	while(scrolling){
		lv_timer_handler();
	}

	lv_obj_t* prObj = lv_obj_get_child(obj, actionCursor);
	lv_obj_set_style_text_color(lv_obj_get_child(lv_obj_get_child(prObj, 1), 0), lv_color_white(), 0);
	lv_obj_remove_style(lv_obj_get_child(prObj, 0), &border, 0);
	lv_obj_add_style(lv_obj_get_child(prObj, 0), &noBorder, 0);

	if(actionCursor == program.actions.size() - 1){
		lv_obj_scroll_by(obj, 0, -(lv_obj_get_height(prObj) + lv_obj_get_style_pad_row(obj, LV_PART_MAIN)), LV_ANIM_ON);
		lv_obj_add_event_cb(obj, [](lv_event_t* e){
			*((bool*) e->user_data) = false;
			lv_obj_remove_event_cb(e->target, nullptr);
		}, LV_EVENT_SCROLL_END, &scrolling);
		scrolling = true;

		lv_obj_set_style_text_color(doneLabel, lv_color_make(255, 0, 0), 0);
	}else{
//		lv_obj_scroll_to_view(prObj, LV_ANIM_ON);
		lv_obj_scroll_by(obj, 0, -(lv_obj_get_height(prObj) + lv_obj_get_style_pad_row(obj, LV_PART_MAIN)), LV_ANIM_ON);
		lv_obj_add_event_cb(obj, [](lv_event_t* e){
			*((bool*) e->user_data) = false;
			lv_obj_remove_event_cb(e->target, nullptr);
		}, LV_EVENT_SCROLL_END, &scrolling);
		scrolling = true;
		actionCursor++;
		prObj = lv_obj_get_child(obj, actionCursor);

		lv_obj_set_style_text_color(lv_obj_get_child(lv_obj_get_child(prObj, 1), 0), lv_color_make(255, 0, 0), 0);
		lv_obj_remove_style(lv_obj_get_child(prObj, 0), &noBorder, 0);
		lv_obj_add_style(lv_obj_get_child(prObj, 0), &border, 0);
	}
	lv_obj_update_layout(obj);
}

ProgPlaybackElement::~ProgPlaybackElement(){
	lv_style_reset(&border);
	lv_style_reset(&noBorder);
	lv_obj_remove_event_cb(obj, nullptr);
}
