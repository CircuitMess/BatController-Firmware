#ifndef BATCONTROLLER_FIRMWARE_SPEEDMODAL_H
#define BATCONTROLLER_FIRMWARE_SPEEDMODAL_H

#include "../Interface/LVModal.h"
#include <Input/InputListener.h>
#include <functional>

class [[maybe_unused]] SpeedModal : public LVModal {
public:
	[[maybe_unused]] explicit SpeedModal(LVScreen* parent, std::function<void(uint8_t)> speedCB, uint8_t currentSpeed);

protected:
	void onStart() override;
	void onStop() override;

private:
	std::function<void(uint8_t)> speedCB;
	lv_obj_t* text;
	lv_obj_t* slider;
	lv_timer_t* timeout;

	static constexpr size_t w = 79;
	static constexpr size_t h = 48;

	static constexpr uint8_t sliderRange = 31;

	static constexpr size_t timeoutValue = 3000; //3s timeout after which modal is exited
};

#endif //BATCONTROLLER_FIRMWARE_SPEEDMODAL_H
