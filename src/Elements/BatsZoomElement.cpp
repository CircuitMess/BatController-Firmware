#include "BatsZoomElement.h"
#include <Arduino.h>
#include "BatLogo.h"


BatsZoomElement::BatsZoomElement(lv_obj_t* parent) : LVObject(parent){
	memcpy(buf, logo.data, logo.data_size);

	logoCopy = logo;
	logoCopy.data = buf;

	lv_obj_set_size(obj, w, h);

	img = lv_img_create(obj);

	lv_img_set_src(img, &logoCopy);
	lv_img_set_pivot(img, logoCopy.header.w / 2, logoCopy.header.h / 2);

	lv_img_set_antialias(img, true);

	lv_obj_center(obj);
	lv_obj_center(img);


	lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
	timer = lv_timer_create([](lv_timer_t* t){
		auto obj = (lv_obj_t*) t->user_data;
		lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
		lv_timer_pause(t);
		lv_anim_del(lv_obj_get_child(obj, 0), nullptr);
	}, duration * 2, obj);
}

BatsZoomElement::~BatsZoomElement(){
	lv_anim_del(img, nullptr);
	lv_timer_del(timer);
}

void BatsZoomElement::startAnim(){
	if(lv_anim_count_running()) return;

	lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);

	//zoom
	lv_anim_init(&zoomAnim);
	lv_anim_set_exec_cb(&zoomAnim, [](void* p, int32_t v){
		auto img = (lv_obj_t*) p;
		lv_img_set_zoom(img, v);
//		lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
	});
	lv_anim_set_values(&zoomAnim, 80, 512);
	lv_anim_set_time(&zoomAnim, duration);
	lv_anim_set_playback_time(&zoomAnim, duration);
	lv_anim_set_repeat_count(&zoomAnim, 1);
	lv_anim_set_path_cb(&zoomAnim, lv_anim_path_linear);
	lv_anim_set_var(&zoomAnim, img);
	lv_anim_start(&zoomAnim);

	//rotation
	lv_anim_init(&rotAnim);
	lv_anim_set_var(&rotAnim, img);
	lv_anim_set_exec_cb(&rotAnim, [](void* p, int32_t v){
		auto img = (lv_obj_t*) p;
		lv_img_set_angle(img, v);
	});
	lv_anim_set_values(&rotAnim, 0, 3600);
	lv_anim_set_time(&rotAnim, duration * 2 / rotations);
	lv_anim_set_repeat_count(&rotAnim, rotations);
	lv_anim_start(&rotAnim);


	lv_timer_reset(timer);
	lv_timer_resume(timer);
}

void BatsZoomElement::stop(){
	lv_timer_reset(timer);
	lv_timer_pause(timer);
	lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
}
