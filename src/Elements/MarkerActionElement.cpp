#include "MarkerActionElement.h"
#include <map>

static const std::map<MarkerAction, const char*> images = {{ MarkerAction::Backward, "S:/DriveScreen/MarkerBackward.bin" },
														   { MarkerAction::Forward,  "S:/DriveScreen/MarkerForward.bin" },
														   { MarkerAction::Do360,    "S:/DriveScreen/MarkerRotate.bin" },
														   { MarkerAction::Burnout,  "S:/DriveScreen/MarkerBurnout.bin" }};


MarkerActionElement::MarkerActionElement(lv_obj_t* parent, MarkerAction action) : LVObject(parent){
	lv_obj_add_flag(obj, LV_OBJ_FLAG_FLOATING);
	lv_obj_add_flag(obj, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_pos(obj, x, y);

	img = lv_img_create(obj);

	setAction(action);
}

void MarkerActionElement::setAction(MarkerAction action){
	if(!images.count(action)){
		lv_img_set_src(img, "");
		lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
		return;
	}

	lv_img_set_src(img, images.at(action));
	lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
}
