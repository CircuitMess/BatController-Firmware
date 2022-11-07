#ifndef BATCONTROLLER_FIRMWARE_HUEMODAL_H
#define BATCONTROLLER_FIRMWARE_HUEMODAL_H

#include "../Interface/LVModal.h"
#include <Input/InputListener.h>
#include <functional>

class HueModal : public LVModal, private InputListener {
public:
	explicit HueModal(LVScreen* parent, std::function<void(uint8_t)> hueCB, uint8_t currentHue);

protected:
	void onStart() override;
	void onStop() override;

private:
	void buttonReleased(uint i) override;
	void buttonHeld(uint i) override;
	void buttonHeldRepeat(uint i, uint repeatCount) override;
	bool heldThresh = false;
	uint8_t increment = 1;
	uint8_t incrementStepUsage = 0;
	static constexpr uint8_t incrementStep = 4; //first 4 steps will be incremented by 1, then the next 4 steps will be incremented by 2, then by 4 etc.

	static constexpr uint holdTime = 300; //time to register long press [ms]
	static constexpr uint holdRepeatTime = 1; //repeated time after held for 'holdTime' [ms]
	std::function<void(uint8_t)> hueCB;
	lv_obj_t* text;
	lv_obj_t* slider;
	lv_obj_t* knobCircle;

	//both events needed for smooth tracking of colored knobCircle element with slider knob part
	static void knobCircleEventDraw(lv_event_t*);
	static void knobCircleEventValue(lv_event_t*);

	static constexpr size_t w = 79;
	static constexpr size_t h = 48;
};


#endif //BATCONTROLLER_FIRMWARE_HUEMODAL_H
