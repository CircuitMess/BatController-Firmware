#include "ProgEditScreen.h"
#include <Pins.hpp>
#include <Input/Input.h>
#include <InputLVGL.h>
#include "SimpleProgScreen.h"
#include "../PairScreen.h"
#include <Com/Communication.h>
#include <FSLVGL.h>

static const std::map<Simple::Action::Type, const char*> actionIcons = {
		{ Simple::Action::Type::Drive,       "S:/SimpleProg/Drive.bin" },
		{ Simple::Action::Type::Headlights,  "S:/SimpleProg/Frontlight.bin" },
		{ Simple::Action::Type::Taillights,  "S:/SimpleProg/Backlight.bin" },
		{ Simple::Action::Type::Underlights, "S:/SimpleProg/Underlight.bin" },
		{ Simple::Action::Type::Sound,       "S:/SimpleProg/Sound.bin" },
		{ Simple::Action::Type::Delay,       "S:/SimpleProg/Delay.bin" }
};


ProgEditScreen::ProgEditScreen(const Simple::Program& program, std::function<void(Simple::Program)> saveCallback) : program(program), editModal(this),
																													pickModal(this),
																													saveCallback(saveCallback){
	lv_obj_set_style_bg_color(obj, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_STATE_DEFAULT);
	lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	// sue me
	lv_obj_t* pad = lv_obj_create(obj);
	lv_obj_set_size(pad, 160, 8);

	actionView = lv_obj_create(obj);
	lv_obj_set_size(actionView, 137, 102);
	lv_obj_set_scrollbar_mode(actionView, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_layout(actionView, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(actionView, LV_FLEX_FLOW_ROW_WRAP);
	lv_obj_set_flex_align(actionView, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_row(actionView, 5, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_column(actionView, 5, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_ver(actionView, 5, LV_STATE_DEFAULT);
	lv_obj_set_style_pad_hor(actionView, 1, LV_STATE_DEFAULT);

	progDeleteTimer = lv_timer_create([](lv_timer_t* timer){
		auto& screen = *(ProgEditScreen*) timer->user_data;

		auto index = lv_obj_get_index(lv_group_get_focused(screen.inputGroup));
		if(index >= screen.program.actions.size()) return;

		screen.program.actions.erase(screen.program.actions.begin() + index);
		lv_obj_del_async(lv_group_get_focused(screen.inputGroup));
		lv_timer_pause(timer);
	}, holdTime, this);
	lv_timer_pause(progDeleteTimer);

	for(auto& action: program.actions){
		addAction(action);
	}

	addNewActionButton();

	for(int j = 0; j < lv_obj_get_child_cnt(actionView); ++j){
		lv_obj_add_event_cb(lv_obj_get_child(actionView, j), [](lv_event_t* e){
			auto screen = (ProgEditScreen*) e->user_data;
			auto key = lv_event_get_key(e);
			if(key == LV_KEY_UP && lv_obj_get_index(e->target) >= ProgEditScreen::rowLength){
				lv_group_focus_obj(lv_obj_get_child(screen->actionView, lv_obj_get_index(e->target) - ProgEditScreen::rowLength));
			}else if(key == LV_KEY_DOWN && (lv_obj_get_child_cnt(screen->actionView) - lv_obj_get_index(e->target) >= ProgEditScreen::rowLength)){
				lv_group_focus_obj(lv_obj_get_child(screen->actionView, lv_obj_get_index(e->target) + ProgEditScreen::rowLength));
			}else if(key == LV_KEY_HOME){
				reinterpret_cast<SimpleProgScreen*>(screen->parent)->setInfoElement(std::move(screen->infoElement));
				screen->pop();
			}
		}, LV_EVENT_KEY, this);
	}

	footer = lv_img_create(obj);
	lv_img_set_src(footer, "S:/SimpleProg/footer_2.bin");

	lv_obj_scroll_to_view(lv_obj_get_child(actionView, 0), LV_ANIM_OFF);
	lv_group_set_focus_cb(inputGroup, [](lv_group_t* g){
		lv_obj_scroll_to_view(lv_group_get_focused(g), LV_ANIM_ON);
	});
}

ProgEditScreen::~ProgEditScreen(){
	lv_timer_del(progDeleteTimer);
}

void ProgEditScreen::onStarting(){
	if(infoElement == nullptr){
		infoElement = std::make_unique<GeneralInfoElement>(getLvObj(), DriveMode::SimpleProgramming);
	}

//	FSLVGL::loadFleha();
}

void ProgEditScreen::onStart(){
	InputLVGL::enableVerticalNavigation(false);
	InputLVGL::enableHorizontalNavigation(true);
	Input::getInstance()->addListener(this);
	Com.addDcListener(this);
}

void ProgEditScreen::onStop(){
	lv_timer_pause(progDeleteTimer);
	editModal.stop();
	pickModal.stop();
	InputLVGL::enableVerticalNavigation(true);
	InputLVGL::enableHorizontalNavigation(false);
	Input::getInstance()->removeListener(this);
	Com.removeDcListener(this);

	bool allStopped = false;
	while(!allStopped){
		for(int j = 0; j < lv_obj_get_child_cnt(actionView); ++j){
			if(!lv_obj_remove_event_cb(lv_obj_get_child(actionView, j), nullptr)){
				allStopped = true;
				break;
			}
		}
	}

	if(saveCallback) saveCallback(program);

//	FSLVGL::unloadFleha();
}

void ProgEditScreen::setInfoElement(std::unique_ptr<GeneralInfoElement> infoElement){
	if(infoElement == nullptr){
		this->infoElement.reset();
		return;
	}

	this->infoElement = std::move(infoElement);
	this->infoElement->setMode(DriveMode::SimpleProgramming);
	lv_obj_set_parent(this->infoElement->getLvObj(), getLvObj());
}

void ProgEditScreen::buttonPressed(uint i){
	if(editModal.isActive() || pickModal.isActive()) return;

	if(i != BTN_B){
		lv_timer_pause(progDeleteTimer);
		return;
	}else{
		backClickTimer = millis();
		if(lv_obj_get_index(lv_group_get_focused(inputGroup)) >= program.actions.size()) return;
	}

	lv_timer_reset(progDeleteTimer);
	lv_timer_resume(progDeleteTimer);
}

void ProgEditScreen::buttonReleased(uint i){
	if(editModal.isActive() || pickModal.isActive()) return;

	if(i == BTN_B && millis() - backClickTimer <= clickTimeMax){
		reinterpret_cast<SimpleProgScreen*>(parent)->setInfoElement(std::move(infoElement));
		pop();
		return;
	}

	lv_timer_pause(progDeleteTimer);
}

void ProgEditScreen::addAction(const Simple::Action& action){
	lv_obj_t* actionElement = lv_img_create(actionView);
	lv_img_set_src(actionElement, actionIcons.at(action.type));
	lv_obj_set_style_radius(actionElement, 1, LV_STATE_DEFAULT);

	lv_group_add_obj(inputGroup, actionElement);
	lv_obj_set_style_outline_color(actionElement, lv_color_white(), LV_STATE_FOCUSED);
	lv_obj_set_style_outline_opa(actionElement, LV_OPA_COVER, LV_STATE_FOCUSED);
	lv_obj_set_style_outline_width(actionElement, 2, LV_STATE_FOCUSED);

	lv_obj_add_event_cb(actionElement, [](lv_event_t* e){
		auto index = lv_obj_get_index(e->target);
		auto& screen = *(ProgEditScreen*) e->user_data;
		auto& action = screen.program.actions[index];

		screen.editModal.startEdit(action);
	}, LV_EVENT_PRESSED, this);
}

void ProgEditScreen::addNewActionButton(){
	newAction = lv_obj_create(actionView);
	lv_obj_set_size(newAction, 15, 15);
	lv_obj_set_style_radius(newAction, 1, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(newAction, LV_OPA_COVER, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(newAction, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(newAction, lv_color_make(0, 190, 214), LV_STATE_FOCUSED);
	lv_group_add_obj(inputGroup, newAction);
	lv_obj_set_style_outline_color(newAction, lv_color_white(), LV_STATE_FOCUSED);
	lv_obj_set_style_outline_opa(newAction, LV_OPA_COVER, LV_STATE_FOCUSED);
	lv_obj_set_style_outline_width(newAction, 2, LV_STATE_FOCUSED);
	lv_obj_set_style_border_width(newAction, 1, LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(newAction, LV_OPA_COVER, LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(newAction, lv_color_white(), LV_STATE_DEFAULT);
	lv_obj_t* newLabel = lv_label_create(newAction);
	lv_obj_center(newLabel);
	lv_obj_set_style_text_font(newLabel, &lv_font_montserrat_10, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(newLabel, lv_color_white(), LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(newLabel, lv_color_black(), LV_STATE_FOCUSED);
	lv_label_set_text(newLabel, LV_SYMBOL_PLUS);

	lv_obj_add_event_cb(newAction, [](lv_event_t* e){
		auto screen = (ProgEditScreen*) e->user_data;

		screen->pickModal.startPick([screen](Simple::Action action){
			screen->program.actions.push_back(action);

			lv_obj_del_async(screen->newAction);
			screen->addAction(action);
			screen->addNewActionButton();

			auto lastIndex = lv_obj_get_child_cnt(screen->actionView) - 1;
			for(int i = lastIndex; i >= lastIndex - 1; --i){
				lv_obj_add_event_cb(lv_obj_get_child(screen->actionView, i), [](lv_event_t* e){
					auto screen = (ProgEditScreen*) e->user_data;
					auto key = lv_event_get_key(e);
					if(key == LV_KEY_UP && lv_obj_get_index(e->target) >= ProgEditScreen::rowLength){
						lv_group_focus_obj(lv_obj_get_child(screen->actionView, lv_obj_get_index(e->target) - ProgEditScreen::rowLength));
					}else if(key == LV_KEY_DOWN && (lv_obj_get_child_cnt(screen->actionView) - lv_obj_get_index(e->target) >= ProgEditScreen::rowLength)){
						lv_group_focus_obj(lv_obj_get_child(screen->actionView, lv_obj_get_index(e->target) + ProgEditScreen::rowLength));
					}else if(key == LV_KEY_HOME){
						reinterpret_cast<SimpleProgScreen*>(screen->parent)->setInfoElement(std::move(screen->infoElement));
						screen->pop();
					}
				}, LV_EVENT_KEY, screen);
			}
			lv_group_focus_obj(lv_obj_get_child(screen->actionView, lv_obj_get_child_cnt(screen->actionView) - 1));
		});

	}, LV_EVENT_PRESSED, this);

}

void ProgEditScreen::onDisconnected(){
	stop();
	delete parent;
	delete this;

//	FSLVGL::unloadFleha();
//	FSLVGL::unloadSimple();

	auto pair = new PairScreen(true);
	pair->start();
}
