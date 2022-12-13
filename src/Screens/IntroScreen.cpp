#include "IntroScreen.h"
#include "../Screens/PairScreen.h"
#include <Loop/LoopManager.h>

static const uint16_t FrameDurations[] = { 200, 150, 150, 50, 50, 50, 50, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 800 };

IntroScreen::IntroScreen(void (* callback)()) : callback(callback){
	img = lv_img_create(obj);
	lv_img_set_src(img, "S:/Intro/1.bin");
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
        LoopManager::defer([pair](uint32_t){
            lv_obj_del(lv_scr_act());
            pair->start();
        });
		return;
	}

	frameTime = time;

	char path[32];
	sprintf(path, "S:/Intro/%d.bin", currentFrame+1);
	lv_img_set_src(img, path);
	lv_obj_invalidate(img);
}
