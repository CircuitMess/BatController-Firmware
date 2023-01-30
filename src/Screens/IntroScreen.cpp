#include "IntroScreen.h"
#include "../Screens/PairScreen.h"
#include <Loop/LoopManager.h>

static const uint16_t FrameDurations[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
                                          100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

IntroScreen::IntroScreen(void (* callback)()) : callback(callback){
	img = lv_img_create(obj);
	lv_img_set_src(img, "S:/Intro/0.bin");
	lv_obj_set_size(img, 160, 128);
}

void IntroScreen::onStart(){
	frameTime = millis();
	LoopManager::addListener(this);
}

void IntroScreen::onStop(){
	LoopManager::removeListener(this);
}

void IntroScreen::loop(uint micros){
	uint32_t time = millis();
	if(time - frameTime < FrameDurations[currentFrame]) return;

	currentFrame++;
	if(currentFrame >= sizeof(FrameDurations) / sizeof(FrameDurations[0])){
		stop();
		volatile auto callback = this->callback;
		delete this;

		if(callback){
			callback();
		}

        auto scr = lv_obj_create(nullptr);
        lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
        lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
        lv_scr_load(scr);

		auto pair = new PairScreen();
        LoopManager::defer([pair, scr](uint32_t){
            pair->start(true, LV_SCR_LOAD_ANIM_FADE_ON);
			auto timer = lv_timer_create([](lv_timer_t* timer){ lv_obj_del((lv_obj_t*) timer->user_data); }, 600, scr);
			lv_timer_set_repeat_count(timer, 1);
        });
		return;
	}

	frameTime = time;

	char path[32];
	sprintf(path, "S:/Intro/%d.bin", currentFrame+1);
	lv_img_set_src(img, path);
	lv_obj_invalidate(img);
}
