#ifndef BATCONTROLLER_FIRMWARE_HUEMODAL_H
#define BATCONTROLLER_FIRMWARE_HUEMODAL_H

#include "../Interface/LVModal.h"
#include <Input/InputListener.h>
#include <functional>

class HueModal : public LVModal {
public:
	explicit HueModal(LVScreen* parent, std::function<void(uint8_t)> hueCB, uint8_t currentHue);
	~HueModal() override;
protected:
	void onStart() override;
	void onStop() override;

private:
	std::function<void(uint8_t)> hueCB;
	lv_obj_t* text;
	lv_obj_t* slider;
	lv_obj_t* knobCircle;
	lv_timer_t* timeout = nullptr;

	static constexpr size_t w = 79;
	static constexpr size_t h = 48;

	static constexpr uint8_t sliderRange = 30;

	static constexpr size_t timeoutValue = 3000; //3s timeout after which modal is exited
};


#endif //BATCONTROLLER_FIRMWARE_HUEMODAL_H
