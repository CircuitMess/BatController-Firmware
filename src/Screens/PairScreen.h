#ifndef BATCONTROLLER_FIRMWARE_PAIRSCREEN_H
#define BATCONTROLLER_FIRMWARE_PAIRSCREEN_H

#include <Display/Sprite.h>
#include "../Interface/LVScreen.h"
#include "../PairService.h"

class PairScreen : public LVScreen{
public:
	PairScreen();
	~PairScreen() override;

	void onStart() override;
	void onStop() override;

private:
	lv_obj_t* canvas;
	lv_obj_t* bg;
	lv_obj_t* text;

	lv_anim_t* anim_drop;

	lv_color_t* buffer;
	lv_color_t black;
	lv_color_t white;

	static constexpr uint8_t Scale = 10;

	PairService pair;
};


#endif //BATCONTROLLER_FIRMWARE_PAIRSCREEN_H
