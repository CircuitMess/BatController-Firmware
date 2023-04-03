#include "IntroScreen.h"
#include "../Screens/PairScreen.h"
#include <Loop/LoopManager.h>
#include <Settings.h>
#include <BatController.h>

static const uint16_t FrameDurations[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
                                          100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

IntroScreen::IntroScreen() : gif(obj, "S:/Intro"){
}

void IntroScreen::onStart(){
	frameTime = millis();
	LoopManager::addListener(this);
	fade = 0;
	gif.start();
	gif.setDoneCallback([this](){
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
				lv_timer_del(timer);
			}, 600, scr);
		});
		return;
	});
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
	}else if(!blInited){
		blInited = true;
		BatController.setBrightness(Settings.get().screenBrightness);
	}
}

void IntroScreen::setPreCallback(std::function<void()> preCallback){
	IntroScreen::preCallback = std::move(preCallback);
}

void IntroScreen::setPostCallback(std::function<void()> postCallback){
	IntroScreen::postCallback = std::move(postCallback);
}
