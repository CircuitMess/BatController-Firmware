#include "SimpleProgScreen.h"
#include "../../Driver/SimpleProgDriver.h"
#include "../DriveScreen.h"
#include "../PairScreen.h"
#include "ProgEditScreen.h"
#include "../MainMenu.h"
#include <Input/Input.h>
#include <Pins.hpp>
#include <Loop/LoopManager.h>
#include "../../FSLVGL.h"

std::array<const char*, 9> SimpleProgScreen::cacheFiles{
		"/SimpleProg/Backlight.bin",
		"/SimpleProg/Delay.bin",
		"/SimpleProg/Drive.bin",
		"/SimpleProg/Frontlight.bin",
		"/SimpleProg/Sound.bin",
		"/SimpleProg/Underlight.bin",
		"/SimpleProg/new.bin",
		"/SimpleProg/newFocused.bin",
		"/SimpleProg/footer.bin"
};

std::array<const char*, 18> SimpleProgScreen::uncachedFiles{ "/Menu/Small/Ball_b.bin",
															 "/Menu/Small/Ball_r.bin",
															 "/Menu/Small/SimpleProg_b.bin",
															 "/Menu/Small/SimpleProg_r.bin",
															 "/Menu/Small/Dance_b.bin",
															 "/Menu/Small/Dance_r.bin",
															 "/Menu/Small/Manual_b.bin",
															 "/Menu/Small/Manual_r.bin",
															 "/Menu/Small/Marker_b.bin",
															 "/Menu/Small/Marker_r.bin",
															 "/Menu/Small/Settings_b.bin",
															 "/Menu/Small/Settings_r.bin",
															 "/Menu/Label/Ball.bin",
															 "/Menu/Label/SimpleProg.bin",
															 "/Menu/Label/Dance.bin",
															 "/Menu/Label/Manual.bin",
															 "/Menu/Label/Marker.bin",
															 "/Menu/Label/Settings.bin" };

uint8_t SimpleProgScreen::lastProgramIndex = 0;

SimpleProgScreen::SimpleProgScreen() : infoElement(obj, DriveMode::SimpleProgramming){

	FSLVGL::addCache(cacheFiles);

	lv_obj_set_style_bg_color(obj, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_STATE_DEFAULT);

	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	// sue me
	lv_obj_t* pad = lv_obj_create(obj);
	lv_obj_set_size(pad, 160, 8);

	progView = lv_obj_create(obj);
	lv_obj_set_size(progView, 160, 102);
	lv_obj_set_layout(progView, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(progView, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(progView, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_row(progView, spaceBetweenProgs, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(progView, 5, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(progView, 3, LV_STATE_DEFAULT);
	lv_obj_set_scrollbar_mode(progView, LV_SCROLLBAR_MODE_ON);

	footer = lv_img_create(obj);
	lv_img_set_src(footer, "S:/SimpleProg/footer.bin");

	progDeleteTimer = lv_timer_create([](lv_timer_t* timer){
		auto& screen = *(SimpleProgScreen*) timer->user_data;
		auto bg = lv_obj_get_child(lv_group_get_focused(screen.inputGroup), 0);
		lv_obj_set_width(bg, map(millis() - screen.holdStartTime, 0, holdTime, 0, programWidth));

		if(millis() - screen.holdStartTime >= holdTime){
			auto index = lv_obj_get_index(lv_group_get_focused(screen.inputGroup));
			screen.storage.removeProg(index);
			lv_obj_del_async(lv_group_get_focused(screen.inputGroup));
			lv_timer_pause(timer);
		}
	}, 1, this);
	lv_timer_pause(progDeleteTimer);
}

void SimpleProgScreen::onStart(){
	FSLVGL::removeCache(uncachedFiles);
	Input::getInstance()->addListener(this);
}

void SimpleProgScreen::onStop(){
	Input::getInstance()->removeListener(this);
}

void SimpleProgScreen::onDisconnected(){
	stop();
	delete this;

	FSLVGL::addCache(uncachedFiles);

	auto pair = new PairScreen();
	pair->start();
}

void SimpleProgScreen::buttonPressed(uint i){
	if(i != BTN_B) return;
	if(!lv_obj_get_child_cnt(lv_group_get_focused(inputGroup))) return;
	auto bg = lv_obj_get_child(lv_group_get_focused(inputGroup), 0);

	if(progDeleteTimer->paused){
		lv_obj_set_style_bg_color(bg, lv_color_make(255, 0, 0), LV_STATE_DEFAULT);
		lv_timer_reset(progDeleteTimer);
		lv_timer_resume(progDeleteTimer);
		holdStartTime = millis();
	}
}

void SimpleProgScreen::buttonReleased(uint i){
	if(!lv_obj_get_child_cnt(lv_group_get_focused(inputGroup))) return;

	auto bg = lv_obj_get_child(lv_group_get_focused(inputGroup), 0);
	lv_obj_set_width(bg, 0);
	holdStartTime = millis();
	lv_timer_pause(progDeleteTimer);
}

void SimpleProgScreen::buildProgView(){
	for(int j = lv_obj_get_child_cnt(progView) - 1; j >= 0; --j){
		lv_obj_del(lv_obj_get_child(progView, j));
	}

	lv_obj_t* progElement;
	for(int i = 0; i < storage.getNumProgs(); ++i){
		auto prog = storage.getProg(i);

		progElement = lv_btn_create(progView);
		lv_obj_set_style_border_width(progElement, 1, LV_STATE_DEFAULT);
		lv_obj_set_style_border_opa(progElement, LV_OPA_COVER, LV_STATE_DEFAULT);
		lv_obj_set_style_border_color(progElement, lv_color_white(), LV_STATE_DEFAULT);

		lv_obj_set_style_outline_width(progElement, 2, LV_STATE_FOCUSED);
		lv_obj_set_style_outline_opa(progElement, LV_OPA_COVER, LV_STATE_FOCUSED);
		lv_obj_set_style_outline_color(progElement, lv_color_white(), LV_STATE_FOCUSED);
		lv_obj_set_style_radius(progElement, 3, LV_STATE_DEFAULT);

		lv_obj_set_size(progElement, programWidth, programHeight);

		lv_obj_t* bg = lv_obj_create(progElement);
		lv_obj_set_size(bg, 0, programHeight - 2);
		lv_obj_set_pos(bg, 0, 0);
		lv_obj_set_style_bg_color(bg, lv_color_make(0, 190, 214), LV_STATE_DEFAULT);
		lv_obj_set_style_bg_opa(bg, LV_OPA_COVER, LV_STATE_DEFAULT);
		lv_obj_set_style_radius(bg, 3, LV_STATE_DEFAULT);

		lv_obj_t* label;
		label = lv_label_create(progElement);
		lv_label_set_text(label, prog.name.c_str());
		lv_obj_center(label);
		lv_obj_set_style_text_font(label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(progElement, lv_color_white(), LV_STATE_DEFAULT);
		lv_group_add_obj(inputGroup, progElement);


		lv_obj_add_event_cb(progElement, [](lv_event_t* e){
			if(!lv_obj_get_child_cnt(e->target)) return;

			auto bg = lv_obj_get_child(e->target, 0);
			lv_obj_set_width(bg, 0);
		}, LV_EVENT_DEFOCUSED, nullptr);


		lv_obj_add_event_cb(progElement, [](lv_event_t* e){
			auto bg = lv_obj_get_child(e->target, 0);
			auto& screen = *(SimpleProgScreen*) e->user_data;

			lv_obj_set_style_bg_color(bg, lv_color_make(0, 190, 214), LV_STATE_DEFAULT);

			screen.holdStartTime = millis();

		}, LV_EVENT_PRESSED, this);

		lv_obj_add_event_cb(progElement, [](lv_event_t* e){
			auto p = (SimpleProgScreen*) e->user_data;
			auto t = p->holdStartTime;
			auto bg = lv_obj_get_child(e->target, 0);

			lv_obj_set_width(bg, map(millis() - t, 0, holdTime, 0, programWidth));
			if(millis() - t >= holdTime){
				//Playback program
				auto index = lv_obj_get_index(e->target);
				SimpleProgScreen::lastProgramIndex = index;

				std::unique_ptr<Simple::Program> program = std::make_unique<Simple::Program>(p->storage.getProg(index));
				std::unique_ptr<SimpleProgDriver> driver = std::make_unique<SimpleProgDriver>(std::move(program));

				p->stop();
				delete p;
				auto ds = new DriveScreen(DriveMode::SimpleProgramming, std::move(driver));
				ds->start();

			}
		}, LV_EVENT_PRESSING, this);

		lv_obj_add_event_cb(progElement, [](lv_event_t* e){
			//Editing existing program
			auto& screen = *(SimpleProgScreen*) e->user_data;
			auto index = lv_obj_get_index(lv_group_get_focused(screen.inputGroup));

			SimpleProgScreen::lastProgramIndex = index;
			screen.currentProgram = screen.storage.getProg(index);
			auto next = new ProgEditScreen(screen.currentProgram, [&screen](){
				screen.storage.updateProg(SimpleProgScreen::lastProgramIndex, screen.currentProgram);
			});
			screen.push(next);
		}, LV_EVENT_SHORT_CLICKED, this);
	}

	newProg = lv_obj_create(progView);
	lv_obj_set_size(newProg, 35, 19);
	lv_obj_set_style_bg_img_src(newProg, "S:/SimpleProg/new.bin", LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_src(newProg, "S:/SimpleProg/newFocused.bin", LV_STATE_FOCUSED);
	lv_group_add_obj(inputGroup, newProg);

	lv_obj_add_event_cb(newProg, [](lv_event_t* e){
		//Adding new program
		auto& screen = *(SimpleProgScreen*) e->user_data;
		auto& prog = screen.currentProgram;

		prog = Simple::Program{};
		prog.name = "Program ";
		prog.name.push_back((char) ((lv_obj_get_index(e->target) + 1) + '0'));
		auto progEdit = new ProgEditScreen(prog, [&screen](){
			screen.storage.addProg(screen.currentProgram);
		});
		screen.push(progEdit);
	}, LV_EVENT_PRESSED, this);

	for(int j = 0; j < lv_obj_get_child_cnt(progView); ++j){
		lv_obj_add_event_cb(lv_obj_get_child(progView, j), [](lv_event_t* e){
			if(lv_event_get_key(e) != LV_KEY_HOME) return;

			auto screen = (SimpleProgScreen*) e->user_data;
			SimpleProgScreen::lastProgramIndex = 0;
			screen->stop();
			delete screen;

			FSLVGL::removeCache(cacheFiles);
			FSLVGL::addCache(uncachedFiles);
			auto mainMenu = new MainMenu();
			mainMenu->start();
		}, LV_EVENT_KEY, this);
	}
}

void SimpleProgScreen::onStarting(){
	buildProgView();

	lv_group_set_focus_cb(inputGroup, [](lv_group_t* g){
		lv_obj_scroll_to_view(lv_group_get_focused(g), LV_ANIM_ON);
	});
	lv_group_focus_obj(lv_obj_get_child(progView, lastProgramIndex));
}
