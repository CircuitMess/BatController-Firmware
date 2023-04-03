#include "LVGIF.h"
#include <SPIFFS.h>

static const char* tag = "LVGIF";

LVGIF::LVGIF(lv_obj_t* parent, const char* path) : LVObject(parent), path(path){
	auto strpath = std::string(path);
	if(strpath.find("S:") == 0){
		strpath = strpath.substr(2);
	}
	auto descPath = strpath + "/desc.bin";
	pathLen = descPath.length() + 10;
	imgPath = new char[pathLen];
	auto f = SPIFFS.open(descPath.c_str());

	if(!f || f.size() == 0){
		ESP_LOGE(tag, "Couldn't open GIF descriptor file at %s", descPath.c_str());
		return;
	}

	//read w and h as uint16_t variables, most significant byte is read first
	f.read(((uint8_t*) &w) + 1, 1);
	f.read(((uint8_t*) &w), 1);
	f.read(((uint8_t*) &h) + 1, 1);
	f.read(((uint8_t*) &h), 1);
	uint32_t length;
	f.read(((uint8_t*) &length) + 3, 1);
	f.read(((uint8_t*) &length) + 2, 1);
	f.read(((uint8_t*) &length) + 1, 1);
	f.read(((uint8_t*) &length), 1);
	durations.reserve(length);
	for(int i = 0; i < length; i++){
		uint16_t duration;
		f.read(((uint8_t*) &duration) + 1, 1);
		f.read(((uint8_t*) &duration), 1);
		durations.push_back(duration * 10);
	}

	lv_obj_set_size(obj, w, h);
	img = lv_img_create(parent);


}

LVGIF::~LVGIF(){
	lv_timer_del(timer);
	delete imgPath;
}

void LVGIF::start(){
	sprintf(imgPath, "%s/%d.bin", path, index);
	lv_img_set_src(img, imgPath);
	timer = lv_timer_create([](lv_timer_t* t){
		auto gif = (LVGIF*) t->user_data;
		gif->index++;
		if(gif->index >= gif->durations.size()){
			gif->cb();
			return;
		}

		lv_timer_set_period(gif->timer, gif->durations[gif->index]);
		sprintf(gif->imgPath, "%s/%d.bin", gif->path, gif->index);
		lv_img_set_src(gif->img, gif->imgPath);
	}, durations[index], this);
}

void LVGIF::stop(){
	lv_timer_pause(timer);
}

void LVGIF::reset(){
	index = 0;
	sprintf(imgPath, "%s/%d.bin", path, index);
	lv_img_set_src(img, imgPath);
	lv_timer_set_period(timer, durations[index]);
	lv_timer_reset(timer);
	lv_timer_resume(timer);
}

void LVGIF::setLooping(lv_gif_loop_t loop){
	switch(loop){
		case LV_GIF_LOOP_DEFAULT:
		case LV_GIF_LOOP_SINGLE:
			lv_timer_set_repeat_count(timer, 1);
			break;
		case LV_GIF_LOOP_ON:
			lv_timer_set_repeat_count(timer, -1);
			break;
	}
}

void LVGIF::setDoneCallback(std::function<void()> cb){
	this->cb = cb;
}

void LVGIF::setImage(uint index){
	this->index = index;
	sprintf(imgPath, "%s/%d.bin", path, index);
	lv_img_set_src(img, imgPath);
	lv_timer_reset(timer);
	lv_timer_resume(timer);
}

size_t LVGIF::getNumFrames() const{
	return durations.size();
}
