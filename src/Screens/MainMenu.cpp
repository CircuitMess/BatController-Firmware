#include <Loop/LoopManager.h>
#include "MainMenu.h"
#include "DriveScreen.h"
#include <Pins.hpp>
#include <Input/Input.h>
#include <Com/Communication.h>
#include "SettingsScreen.h"
#include "PairScreen.h"
#include "SimpleProg/SimpleProgScreen.h"
#include "../FSLVGL.h"

uint8_t MainMenu::lastSelected = 0;
MainMenu::MainMenu() : LVScreen() {

	selected = lastSelected;

    bigContainers.reserve(ItemCount * 2);
    bigs.reserve(ItemCount * 2);
    bigLabels.reserve(ItemCount * 2);
    smalls.reserve(ItemCount * 2);

    lv_obj_set_size(obj, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);

    right = lv_obj_create(obj);
    top = lv_obj_create(obj);

	midContainer = lv_obj_create(obj);
	mid = lv_obj_create(midContainer);

    lv_obj_set_pos(top, 0, 0);
    lv_obj_set_size(top, lv_pct(100), 8);

	lv_obj_set_pos(right, 147, 8);
    lv_obj_set_size(right, 13, 120);
    lv_obj_set_style_pad_top(right, 3, 0);
    lv_obj_set_style_pad_left(right, 13, 0);
    lv_obj_set_scrollbar_mode(right, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(right, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(right, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

	lv_obj_set_pos(midContainer, 0, 8);
	lv_obj_set_size(midContainer, 147, 120);
	lv_obj_set_style_pad_top(midContainer, 120, 0);
	lv_obj_set_scrollbar_mode(midContainer, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_size(mid, 147, 120);
    lv_obj_set_scrollbar_mode(mid, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(mid, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(mid, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);


    for (const auto &item: Items) {
        lv_obj_t *bigContainer = lv_obj_create(mid);
        lv_obj_t *bigLabel = lv_img_create(bigContainer);
        lv_obj_t *small = lv_img_create(right);

        bigContainers.push_back(bigContainer);
        bigLabels.push_back(bigLabel);
        smalls.push_back(small);

        lv_img_set_src(bigLabel, (String("S:/Menu/Label/") + item + ".bin").c_str());
        lv_img_set_src(small, (String("S:/Menu/Small/") + item + "_b.bin").c_str());

        lv_obj_set_flex_flow(bigContainer, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(bigContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(bigContainer, lv_pct(100));
        lv_obj_set_height(bigContainer, 120);

        // lv_obj_set_style_translate_y(bigContainer, item.offset, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_group_add_obj(inputGroup, bigContainer);

        lv_obj_clear_flag(bigContainer, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_clear_flag(bigContainer, LV_OBJ_FLAG_SCROLLABLE);
    }

    loadGIFs();

    lv_group_set_wrap(inputGroup, false);
    lv_group_focus_obj(bigContainers[selected]);

    inputGroup->user_data = this;
    lv_group_set_focus_cb(inputGroup, [](lv_group_t *group) {
        auto *menu = static_cast<MainMenu *>(group->user_data);
		if(menu->launching) return;

        lv_obj_t *focused = lv_group_get_focused(group);
        uint8_t index = lv_obj_get_index(focused);
        menu->scrollTo(index);
    });
}

void MainMenu::loadGIFs() {
    if (!bigs.empty()) return;

    for (int i = 0; i < ItemCount; i++) {
        lv_obj_t *big = lv_gif_create(bigContainers[i]);
        bigs.push_back(big);

        lv_gif_set_src(bigs[i], (String("S:/Menu/Big/") + Items[i] + ".gif").c_str());
        lv_gif_set_loop(big, LV_GIF_LOOP_ON);
        lv_gif_restart(big);
        lv_gif_stop(big);

        lv_obj_set_style_pad_top(big, 4, 0);
    }
}

void MainMenu::unloadGIFs() {
    for (int i = 0; i < bigs.size(); i++) {
        lv_obj_del(bigs[i]);
    }
	bigs.clear();
}

void MainMenu::setRed(uint8_t index, bool reverse) {
    if (index >= smalls.size()) return;

    if (reverse) {
        lv_img_set_src(smalls[index], (String("S:/Menu/Small/") + Items[index] + "_b.bin").c_str());
        lv_gif_stop(bigs[index]);
    } else {
        lv_img_set_src(smalls[index], (String("S:/Menu/Small/") + Items[index] + "_r.bin").c_str());
        lv_gif_start(bigs[index]);
    }

}

void MainMenu::onStarting() {
	FSLVGL::unloadFleha();
	FSLVGL::unloadSimple();

	if(infoElement == nullptr){
		infoElement = std::make_unique<GeneralInfoElement>(getLvObj());
	}

    if (bigs.empty()) {
        loadGIFs();
    } else {
        for (auto big: bigs) {
            lv_gif_restart(big);
        }
    }

	lv_group_focus_obj(bigs[selected]);
	lv_obj_scroll_to_y(mid, selected * 120, LV_ANIM_OFF);

	lv_obj_scroll_to_x(right, 0, LV_ANIM_OFF);
	lv_obj_scroll_to_y(midContainer, 0, LV_ANIM_OFF);
	setRed(selected);
}

void MainMenu::onStart() {
    lv_gif_start(bigs[selected]);

    Input::getInstance()->addListener(this);
    Com.addDcListener(this);

	lv_obj_scroll_to_x(right, 13, LV_ANIM_ON);
	lv_obj_scroll_to_y(midContainer, 120, LV_ANIM_ON);

	clearInput();
	for(auto bigContainer : bigContainers){
		lv_obj_add_event_cb(bigContainer, [](lv_event_t *e) {
			auto *menu = static_cast<MainMenu *>(e->user_data);
			menu->launch();
		}, LV_EVENT_PRESSED, this);
	}

	Com.sendIdleSound(true);
	launching = false;
}

void MainMenu::onStop() {
	Com.sendIdleSound(false);
	lastSelected = selected;
    for (int i = 0; i < ItemCount; i++) {
        lv_gif_stop(bigs[i]);
    }
    unloadGIFs();
	clearInput();
    Input::getInstance()->removeListener(this);
    Com.removeDcListener(this);
}

void MainMenu::outro(){
	lv_obj_scroll_to_x(right, 0, LV_ANIM_ON);
	lv_obj_scroll_to_y(midContainer, 0, LV_ANIM_ON);
}

void MainMenu::clearInput(){
	for(auto bigContainer : bigContainers){
		lv_obj_remove_event_cb_with_user_data(bigContainer, nullptr, this);
	}
}

void MainMenu::scrollTo(uint8_t index) {
    if (index > selected) {
        selectNext();
    } else if (index < selected) {
        selectPrev();
    }
}

void MainMenu::launch() {
	if(launching) return;
	launching = true;

	clearInput();
	outro();

	auto timer = lv_timer_create([](lv_timer_t* timer){
		auto menu = static_cast<MainMenu*>(timer->user_data);
		lv_timer_del(timer);

		const std::function<LVScreen*(void)> screens[] = {
				[]() -> LVScreen * { return new DriveScreen(DriveMode::Manual); },
				[]() -> LVScreen * { return new SimpleProgScreen; },
				[]() -> LVScreen * { return new DriveScreen(DriveMode::Dance); },
				[]() -> LVScreen * { return new DriveScreen(DriveMode::Ball); },
				[]() -> LVScreen * { return new DriveScreen(DriveMode::Marker); },
				[]() -> LVScreen * { return new SettingsScreen(); }
		};
		volatile const auto selected = menu->selected;

		menu->stop();

		// Settings
		if(selected == ItemCount-1){
			auto screen = screens[selected]();
			if(screen == nullptr) return;

			screen->setParent(menu);
			screen->start();
			return;
		}else if(selected == 1){ // Simple prog

			auto info = std::move(menu->infoElement);
			auto tmpScr = lv_obj_create(nullptr);
			lv_obj_set_parent(info->getLvObj(), tmpScr);

			delete menu;

			auto launcher = screens[selected];
			if(launcher == nullptr) return;

			auto screen = reinterpret_cast<SimpleProgScreen*>(launcher());
			screen->setInfoElement(std::move(info));
			lv_obj_del(tmpScr);

			screen->start();

			return;
		}

        GeneralInfoElement* info = menu->infoElement.release();


		auto tmpScr = lv_obj_create(nullptr);
		lv_obj_set_style_bg_color(tmpScr, lv_color_black(), 0);
		lv_obj_set_style_bg_opa(tmpScr, LV_OPA_COVER, 0);
		lv_obj_set_parent(info->getLvObj(), tmpScr);

		delete menu;
		lv_scr_load(tmpScr);

		auto launcher = screens[selected];
		LoopManager::defer([launcher, info, tmpScr](uint32_t){
			DriveScreen* screen = reinterpret_cast<DriveScreen*>(launcher());

			screen->setInfoElement(std::unique_ptr<GeneralInfoElement>(info));
			lv_obj_del(tmpScr);

			screen->start();
		});
	}, 600, this);
}

void MainMenu::selectNext() {
    if (selected + 1 >= ItemCount) return;

    setRed(selected, true);
    selected++;
    setRed(selected);

    lv_obj_scroll_to(mid, 0, selected * 120, LV_ANIM_ON);
}

void MainMenu::selectPrev() {
    if (selected == 0) return;

    setRed(selected, true);
    selected--;
    setRed(selected);

    lv_obj_scroll_to(mid, 0, selected * 120, LV_ANIM_ON);
}

void MainMenu::buttonPressed(uint i) {
//    if(i != BTN_BACK && i != BTN_R) return; TODO: I have no idea what to do with this
//    LockScreen::activate(this);
}

void MainMenu::setInfoElement(std::unique_ptr<GeneralInfoElement> infoElement) {
	if(infoElement == nullptr){
		this->infoElement.reset();
		return;
	}

    this->infoElement = std::move(infoElement);
    this->infoElement->setMode(DriveMode::Idle);
	lv_obj_set_parent(this->infoElement->getLvObj(), getLvObj());
}

void MainMenu::onDisconnected() {
    stop();
    delete this;
	lastSelected = 0;
    auto pair = new PairScreen(true);
    pair->start();
}

void MainMenu::resetLastSelected(){
	lastSelected = 0;
}

