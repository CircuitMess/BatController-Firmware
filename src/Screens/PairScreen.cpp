#include "PairScreen.h"
#include <lvgl.h>
#include <BatController.h>
#include <Aruco/Aruco.h>
#include "DriveScreen.h"
#include "MainMenu.h"


PairScreen::PairScreen() : LVScreen(), /*scanAruco(obj), connecting(obj), error(obj), scanQR(obj),*/ input(obj, inputGroup){
    lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_size(obj, 160, 128);


    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_top(obj, 128, 0);
    lv_obj_scroll_to_y(obj, 0, LV_ANIM_OFF);
}


PairScreen::~PairScreen() {
}

void PairScreen::onStart() {
    int randID = rand() % 256;

//    scanAruco.start(randID);
//    connecting.start();
//    error.start();
//    scanQR.start();
    input.start();

	pair.setDoneCallback([this](){
		stop();
		delete this;

		Serial.printf("Paired\n");
		auto mainMenu = new MainMenu();
		mainMenu->start();
	});

    pair.start(0);
    lv_obj_scroll_to_y(obj, 128, LV_ANIM_ON);
}

void PairScreen::onStop(){
	pair.stop();
}

