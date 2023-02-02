#include "IntroScreen.h"
#include "../Screens/PairScreen.h"
#include <Loop/LoopManager.h>
#include <Settings.h>
#include <BatController.h>

static const uint16_t FrameDurations[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
                                          100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

IntroScreen::IntroScreen(){
	img = lv_img_create(obj);
	lv_img_set_src(img, "S:/Intro/0.bin");
	lv_obj_set_size(img, 160, 128);
}

void IntroScreen::onStart(){
	frameTime = millis();
	LoopManager::addListener(this);
	fade = 0;
}

void IntroScreen::onStop(){
	LoopManager::removeListener(this);
}

void IntroScreen::loop(uint micros){
	if(fade == 0){
		fade = 1;
		ledcSetup(6, 5000, 8);
		ledcWrite(6, 255);
		ledcAttachPin(PIN_BL, 6);
		return;
	}else if(fade < FadeTime){
		fade = min(fade + micros, (uint32_t) FadeTime);
		float t = (float) fade / (float) FadeTime;

		uint8_t maxBrightness = map(Settings.get().screenBrightness, 0, 255, 240, 0);
		uint8_t diff = (255 - maxBrightness);
		uint8_t current = 255.0f - (float) diff * t;


		ledcWrite(6, current);
		printf("PWM %d\n", current);
	}else if(!blInited){
		blInited = true;
		BatController.setBrightness(Settings.get().screenBrightness);
	}

	uint32_t time = millis();
	if(time - frameTime < FrameDurations[currentFrame]) return;

	currentFrame++;
	if(currentFrame >= sizeof(FrameDurations) / sizeof(FrameDurations[0])){
		stop();
		auto preCallback = this->preCallback;
		auto postCallback = this->postCallback;
		delete this;

		if(preCallback){
			preCallback();
		}

        auto scr = lv_obj_create(nullptr);
        lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
        lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
        lv_scr_load(scr);

		auto pair = new PairScreen();
		LoopManager::defer([pair, scr, postCallback](uint32_t){
			pair->start(true, LV_SCR_LOAD_ANIM_FADE_ON);

			if(postCallback){
				postCallback();
			}

			auto timer = lv_timer_create([](lv_timer_t* timer){
				lv_obj_del((lv_obj_t*) timer->user_data);
			}, 600, scr);
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

void IntroScreen::setPreCallback(std::function<void()> preCallback){
	IntroScreen::preCallback = std::move(preCallback);
}

void IntroScreen::setPostCallback(std::function<void()> postCallback){
	IntroScreen::postCallback = std::move(postCallback);
}
