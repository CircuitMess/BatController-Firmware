#ifndef BATCONTROLLER_FIRMWARE_BATSZOOMELEMENT_H
#define BATCONTROLLER_FIRMWARE_BATSZOOMELEMENT_H


#include <LVObject.h>

class BatsZoomElement : public LVObject {
public:
	explicit BatsZoomElement(lv_obj_t* parent);
	~BatsZoomElement() override;

	void stop();
	void startAnim();
private:
	static constexpr uint8_t w = 82;
	static constexpr uint8_t h = 82;
	static constexpr uint32_t duration = 1200;
	static constexpr uint8_t rotations = 4;

	uint8_t buf[2829];
	lv_obj_t* img;
	lv_anim_t zoomAnim;
	lv_anim_t rotAnim;
	lv_timer_t* timer;

	lv_img_dsc_t logoCopy;
};


#endif //BATCONTROLLER_FIRMWARE_BATSZOOMELEMENT_H
