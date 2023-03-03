#ifndef BATCONTROLLER_FIRMWARE_BALLDRIVER_H
#define BATCONTROLLER_FIRMWARE_BALLDRIVER_H

#include "Driver.h"
#include "AutoControls.h"
#include "../Elements/BallHueElement.h"
#include "../Elements/HueModal.h"
#include "../Elements/DanceElement.h"


class BallDriver : public Driver, private InputListener {
public:
	BallDriver(lv_obj_t* container, LVScreen* screen);
	void onFrame(const DriveInfo& frame, Color* pixels) override;

	~BallDriver() override;

protected:
	void onStart() override;
	void onStop() override;

private:
	uint8_t hue = 110;

	AutoControls autoControls;
	BallHueElement hueElement;
	HueModal hueModal;
	lv_obj_t* lockElement;

	bool motorsLock = false;

	void buttonPressed(uint i) override;
};


#endif //BATCONTROLLER_FIRMWARE_BALLDRIVER_H
