#ifndef BATCONTROLLER_FIRMWARE_BATSZOOMELEMENT_H
#define BATCONTROLLER_FIRMWARE_BATSZOOMELEMENT_H


#include "../Interface/LVObject.h"

class BatsZoomElement : public LVObject {
public:
	explicit BatsZoomElement(lv_obj_t* parent);
	~BatsZoomElement() override;

	void startAnim();
private:
	static constexpr uint8_t maxScale = 5;
	static constexpr uint8_t w = 9 * (maxScale + 1) + 1;
	static constexpr uint8_t h = 5 * (maxScale + 1) + 1;
	static constexpr uint32_t duration = 2000;
	lv_obj_t* img;
	lv_anim_t zoomAnim;
	lv_timer_t* timer;
};


#endif //BATCONTROLLER_FIRMWARE_BATSZOOMELEMENT_H
