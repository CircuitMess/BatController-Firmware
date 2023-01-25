#include "SimpleProgDriver.h"
#include "../Screens/SimpleProg/SimpleProgScreen.h"
#include <Loop/LoopManager.h>
#include <Com/Communication.h>
#include <SPIFFS.h>
#include "../Screens/SimpleProg/ActionEditModal.h"

SimpleProgDriver::SimpleProgDriver(std::unique_ptr<Simple::Program> program) : program(std::move(program)){

}

void SimpleProgDriver::setContainer(lv_obj_t* container){
	inited = true;
	//TODO - build UI once we have a container

    panel = lv_obj_create(container);
    lv_obj_set_size(panel,150, 110);
    lv_obj_set_pos(panel,10, 18);
    lv_obj_set_layout(panel, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_snap_y(panel, LV_SCROLL_SNAP_CENTER );
    lv_obj_set_style_pad_gap(panel, 2, 0);
    lv_obj_set_style_pad_bottom(panel, 10, 0);

    lv_style_init(&border);
    lv_style_set_radius(&border, 1);
    lv_style_set_outline_color(&border,lv_palette_main(LV_PALETTE_RED));
    lv_style_set_outline_opa(&border, LV_OPA_COVER);
    lv_style_set_outline_width(&border, 2);

    for(int i = 0; i < program->actions.size(); i++){
        Simple::Action action = program->actions[i];
        lv_obj_t* obj = lv_obj_create(panel);
        lv_obj_set_size(obj, 150, 19);

        lv_obj_t* img = lv_img_create(obj);
        lv_obj_align(img, LV_ALIGN_LEFT_MID, 2, 0);

        lv_obj_t* text = lv_label_create(obj);
        lv_obj_set_size(text,100, 15);
        lv_obj_set_style_text_color(text, lv_color_white(), 0);
        lv_obj_set_style_text_font(text,  &lv_font_montserrat_10, 0);
        lv_obj_set_align(text, LV_ALIGN_CENTER);

        switch(action.type){
            case Simple::Action::Type::Drive:
                lv_img_set_src(img, "S:/SimpleProg/Drive.bin");
                lv_label_set_text(text,( String("Driving ") + DirectionSymbols[(uint8_t)action.DriveData.dir]
                                + " " + (float)action.DriveData.duration/10 + "s").c_str());
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
                lv_label_set_text(text, (String("Under-lights ") + ColorNames[action.RGBData.colorID]).c_str());
                break;
            case Simple::Action::Type::Sound:
                lv_img_set_src(img, "S:/SimpleProg/Sound.bin");
                lv_label_set_text(text, (String("Play sound ") + SoundNames[action.SoundData.sampleIndex]).c_str());
                lv_label_set_long_mode(text, LV_LABEL_LONG_SCROLL_CIRCULAR);
                break;
            case Simple::Action::Type::Delay:
                lv_img_set_src(img, "S:/SimpleProg/Delay.bin");
                lv_label_set_text(text, (String("Delay ") + (float)action.DelayData.duration/10 + "s").c_str());
                break;
        }

    };

    lv_obj_t* tmp;
    tmp = lv_obj_get_child(panel, actionCursor);
    lv_obj_scroll_to(panel,0, actionCursor * 20, LV_ANIM_ON);
    lv_obj_set_style_text_color(lv_obj_get_child(tmp, 1), lv_color_make(255,0,0), 0);
    lv_obj_add_style(lv_obj_get_child(tmp, 0), &border, 0);
}

void SimpleProgDriver::onStart(){
	if(!inited) return;

	actionTimer = 0;
	actionCursor = 0;

	LoopManager::addListener(this);
}

void SimpleProgDriver::onStop(){
	LoopManager::removeListener(this);
}

void SimpleProgDriver::loop(uint micros){

	if(actionCursor >= program->actions.size()){
		stop();
		return;
	}

	auto currentAction = program->actions[actionCursor];

	//timekeeping for actions with durations (Drive and Delay)
	if(currentAction.type == Simple::Action::Type::Drive && actionExecuted){
		actionTimer += micros;
		if(actionTimer >= currentAction.DriveData.duration * 100000){
            nextAction();
			actionTimer = 0;
			actionExecuted = false;
			Com.sendDriveDir((uint8_t) DriveDirection::None);
			return;
		}
	}else if(currentAction.type == Simple::Action::Type::Delay && actionExecuted){
		actionTimer += micros;
		if(actionTimer >= currentAction.DelayData.duration * 100000){
            nextAction();
			actionTimer = 0;
			actionExecuted = false;
			return;
		}
	}


	if(!actionExecuted){
		switch(currentAction.type){
			case Simple::Action::Type::Drive:
				Com.sendDriveSpeed(constrain(currentAction.DriveData.speed, 0, 100));
				Com.sendDriveDir((uint8_t) currentAction.DriveData.dir);
				actionExecuted = true;
				actionTimer = 0;
				break;

			case Simple::Action::Type::Headlights:
				Com.sendHeadlights(currentAction.HeadTaillightData.toggle ? 255 : 0);
                nextAction();
				break;

			case Simple::Action::Type::Taillights:
				Com.sendTaillights(currentAction.HeadTaillightData.toggle ? 255 : 0);
                nextAction();
				break;

			case Simple::Action::Type::Underlights:
				Com.sendUnderlights(currentAction.RGBData.colorID);
                nextAction();
				break;

			case Simple::Action::Type::Sound:
				Com.sendSoundEffect(currentAction.SoundData.sampleIndex);
				break;

			case Simple::Action::Type::Delay:
				actionExecuted = true;
				actionTimer = 0;
				break;
		}
	}
}

void SimpleProgDriver::nextAction() {
    lv_obj_t* tmp = lv_obj_get_child(panel, actionCursor);
    lv_obj_set_style_text_color(lv_obj_get_child(tmp, 1), lv_color_white(), 0);
    lv_obj_remove_style(lv_obj_get_child(tmp, 0), &border, 0);

    actionCursor++;
    lv_obj_scroll_to(panel,0, actionCursor * 20, LV_ANIM_ON);
    tmp = lv_obj_get_child(panel, actionCursor);
    lv_obj_set_style_text_color(lv_obj_get_child(tmp, 1), lv_color_make(255,0,0), 0);
    lv_obj_add_style(lv_obj_get_child(tmp, 0), &border, 0);
}
