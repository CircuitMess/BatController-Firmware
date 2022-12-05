#include <Loop/LoopManager.h>
#include "MainMenu.h"
#include "DriveScreen.h"
#include <Pins.hpp>
#include <Input/Input.h>


const MainMenu::Item MainMenu::Items[] = {
        {"Manual",   0},
        {"Line",     0},
        {"Ball",     0},
        {"Marker",   0},
        {"Settings", 0},
};

const uint8_t MainMenu::ItemCount = sizeof(Items) / sizeof(Items[0]);

MainMenu::MainMenu() : LVScreen() {
    bigContainers.reserve(ItemCount * 2);
    bigs.reserve(ItemCount * 2);
    bigLabels.reserve(ItemCount * 2);
    smalls.reserve(ItemCount * 2);

    lv_obj_set_size(obj, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);

    right = lv_obj_create(obj);
    mid = lv_obj_create(obj);
    top = lv_obj_create(obj);

    lv_obj_set_pos(top, 0, 0);
    lv_obj_set_size(top, lv_pct(100), 8);

    lv_obj_set_pos(right, 147, 8);
    lv_obj_set_size(right, 13, 120);
    lv_obj_set_style_pad_top(right, 3, 0);
    lv_obj_set_scrollbar_mode(right, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(right, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(right, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    lv_obj_set_pos(mid, 0, 8);
    lv_obj_set_size(mid, 147, 120);
//    lv_obj_set_style_pad_left(mid, 3,0);
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

        lv_img_set_src(bigLabel, (String("S:/Menu/Label/") + item.icon + ".bin").c_str());
        lv_img_set_src(small, (String("S:/Menu/Small/") + item.icon + "_b.bin").c_str());

        lv_obj_set_flex_flow(bigContainer, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(bigContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_width(bigContainer, lv_pct(100));
        lv_obj_set_height(bigContainer, 120);

        lv_obj_set_style_translate_y(bigContainer, item.offset, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_group_add_obj(inputGroup, bigContainer);
        lv_obj_add_event_cb(bigContainer, [](lv_event_t *e) {
            auto *menu = static_cast<MainMenu *>(e->user_data);
            menu->launch();
        }, LV_EVENT_PRESSED, this);
        lv_obj_clear_flag(bigContainer, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_clear_flag(bigContainer, LV_OBJ_FLAG_SCROLLABLE);
    }

    loadGIFs();

    lv_group_set_wrap(inputGroup, false);
    lv_group_focus_obj(bigContainers.front());

    inputGroup->user_data = this;
    lv_group_set_focus_cb(inputGroup, [](lv_group_t *group) {
        auto *menu = static_cast<MainMenu *>(group->user_data);
        lv_obj_t *focused = lv_group_get_focused(group);
        uint8_t index = lv_obj_get_index(focused);
        menu->scrollTo(index);
    });


    for (int i = 1; i < ItemCount; i++) {
        lv_obj_add_flag(bigContainers[i], LV_OBJ_FLAG_HIDDEN);
    }

    lv_obj_scroll_by(mid, 0, 128, LV_ANIM_OFF);
}

MainMenu::~MainMenu() {

}

void MainMenu::loadGIFs() {
    if (!bigs.empty()) return;
    for (int i = 0; i < ItemCount; i++) {
        lv_obj_t *big = lv_gif_create(bigContainers[i]);
        bigs.push_back(big);

        lv_gif_set_src(bigs[i], (String("S:/Menu/Big/") + Items[i].icon + ".gif").c_str());
        lv_gif_set_loop(big, LV_GIF_LOOP_ON);
        lv_gif_restart(big);
        lv_gif_stop(big);

        lv_obj_set_style_pad_top(big, 4, 0);
    }
}

void MainMenu::unloadGIFs() {
    for (int i = 0; i < bigs.size(); i++) {
        if(i != selected)
        lv_obj_del(bigs[i]);
    }
}


void MainMenu::unloadLastGIF() {
    lv_obj_del(bigs[selected]);
    bigs.clear();

    delete infoElement;
    infoElement = nullptr;
}

void MainMenu::setRed(uint8_t index, bool reverse) {
    if (index >= smalls.size()) return;

    if (reverse) {
        lv_img_set_src(smalls[index], (String("S:/Menu/Small/") + Items[index].icon + "_b.bin").c_str());
        lv_gif_stop(bigs[index]);
    } else {
        lv_img_set_src(smalls[index], (String("S:/Menu/Small/") + Items[index].icon + "_r.bin").c_str());
        lv_gif_start(bigs[index]);
    }

}

void MainMenu::onStarting() {
    infoElement = new GeneralInfoElement(top, DriveMode::Idle);

    if (bigs.empty()) {
        loadGIFs();
    } else {
        for (auto big: bigs) {
            lv_gif_restart(big);
        }
    }

    if (inited) {
        lv_group_focus_obj(bigs[selected]);
        lv_obj_scroll_to_y(mid, selected * 128, LV_ANIM_OFF);
    }

}

void MainMenu::onStart() {
    lv_gif_start(bigs[selected]);

    if (!inited) {
        setRed(selected);

        lv_obj_add_event_cb(mid, [](lv_event_t *e) {
            auto *menu = static_cast<MainMenu *>(e->user_data);
            lv_obj_remove_event_cb_with_user_data(menu->mid, nullptr, menu);

            for (int i = 1; i < ItemCount; i++) {
                lv_obj_clear_flag(menu->bigContainers[i], LV_OBJ_FLAG_HIDDEN);
            }
        }, LV_EVENT_SCROLL_END, this);

        lv_obj_scroll_to(mid, 0, 0, LV_ANIM_ON);
        inited = true;
    }

    Input::getInstance()->addListener(this);
}

void MainMenu::onStop() {
    for (int i = 0; i < ItemCount; i++) {
        lv_gif_stop(bigs[i]);
    }
    unloadGIFs();
    Input::getInstance()->removeListener(this);
}

void MainMenu::scrollTo(uint8_t index) {
    if (index > selected) {
        selectNext();
    } else if (index < selected) {
        selectPrev();
    }
}

void MainMenu::launch() {
    stop();

    static LVScreen *(*screens[])() = {
            []() -> LVScreen * { return new DriveScreen(DriveMode::Manual); },
            []() -> LVScreen * { return new DriveScreen(DriveMode::Line); }, //TODO: add Line driver
            []() -> LVScreen * { return new DriveScreen(DriveMode::Ball); },
            []() -> LVScreen * { return new DriveScreen(DriveMode::Marker); },
            //[]() -> LVScreen * { return new DummyScreen(); }, TODO: add settings
    };

    LoopManager::defer([this](uint32_t) {
        LVScreen *screen = screens[selected]();
        if (!screen) return;

        push(screen);
        auto timer = lv_timer_create([](lv_timer_t* timer){
            auto menu = static_cast<MainMenu*>(timer->user_data);
            menu->unloadLastGIF();
        }, 500, this);
        lv_timer_set_repeat_count(timer, 1);
    });
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