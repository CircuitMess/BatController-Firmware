#include "BallDriver.h"
#include <Com/Communication.h>
#include <Pins.hpp>
#include <Input/Input.h>

BallDriver::BallDriver(lv_obj_t* container, LVScreen* screen) : Driver(DriveMode::Ball), autoControls(container), hueElement(container, hue),
																hueModal(screen, [this](uint8_t hue){
																	this->hue = hue;
																	hueElement.setHue(hue);
																	Com.sendBallHue(hue);
																}, hue){

}

BallDriver::~BallDriver() {
    stop();
}

void BallDriver::onFrame(const DriveInfo& frame, Color* pixels){
	if(frame.mode != DriveMode::Ball) return;
	autoControls.setDirection(frame.motors);

	if(frame.toBall() == nullptr || frame.toBall()->balls.empty()) return;
	const auto& ball = frame.toBall()->balls.front();

	lv_obj_t* canvas = lv_canvas_create(nullptr);
	lv_canvas_set_buffer(canvas, pixels, 160, 120, LV_IMG_CF_TRUE_COLOR);

	lv_draw_rect_dsc_t draw = {};
	lv_draw_rect_dsc_init(&draw);
	draw.border_width = 1;
	draw.border_color = lv_color_make(255, 0, 0);
	draw.border_opa = LV_OPA_COVER;
	draw.border_side = LV_BORDER_SIDE_FULL;
	draw.bg_opa = LV_OPA_0;
	draw.radius = LV_RADIUS_CIRCLE;
	lv_canvas_draw_rect(canvas, ball.pos.x - ball.r, ball.pos.y - ball.r, ball.r*2, ball.r*2, &draw);

	lv_obj_del(canvas);
}

void BallDriver::buttonPressed(uint i){
	if((i == BTN_LEFT || i == BTN_RIGHT) && !hueModal.isActive()){
		hueModal.start();
	}
}

void BallDriver::onStart(){
	Com.sendBallHue(hue);
	hueElement.setHue(hue);

	autoControls.start();
	Input::getInstance()->addListener(this);
}

void BallDriver::onStop(){
	hueModal.stop();
	autoControls.stop();
	Input::getInstance()->removeListener(this);
}

