#ifndef BATCONTROLLER_FIRMWARE_BALLDRIVER_H
#define BATCONTROLLER_FIRMWARE_BALLDRIVER_H


#include "Driver.h"
#include "AutoControls.h"
#include "../Elements/BallHueElement.h"
#include "../Elements/HueModal.h"

class BallDriver : public Driver, private InputListener {
public:
	BallDriver(lv_obj_t* container, LVScreen* screen);
	void onFrame(const DriveInfo& frame, Color* pixels) override;

private:
	AutoControls autoControls;
	BallHueElement hueElement;
	HueModal hueModal;

	void buttonPressed(uint i) override;
protected:
	void onStart() override;
	void onStop() override;
};


#endif //BATCONTROLLER_FIRMWARE_BALLDRIVER_H
