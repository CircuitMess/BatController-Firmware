#include "LineStatusElement.h"
#include <map>

static const std::map<LineStatus, const char*> images = {{ LineStatus::OffLine, "S:/DriveScreen/LineStatusOff.bin" },
														 { LineStatus::OnLine, "S:/DriveScreen/LineStatusOn.bin" }};

LineStatusElement::LineStatusElement(lv_obj_t* parent, LineStatus status) : LVObject(parent){
	lv_obj_add_flag(obj, LV_OBJ_FLAG_FLOATING);
	lv_obj_add_flag(obj, LV_OBJ_FLAG_IGNORE_LAYOUT);
	lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_pos(obj, x, y);

	img = lv_img_create(obj);
	lv_img_set_src(img, images.at(status));
}

void LineStatusElement::setStatus(LineStatus status){
	lv_img_set_src(img, images.at(status));
}
