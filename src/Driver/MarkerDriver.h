#ifndef BATCONTROLLER_FIRMWARE_MARKERDRIVER_H
#define BATCONTROLLER_FIRMWARE_MARKERDRIVER_H

#include "Driver.h"
#include "AutoControls.h"
#include "../Elements/MarkerActionElement.h"
#include "../Elements/BatsZoomElement.h"
#include "../Elements/DanceElement.h"

class MarkerDriver : public Driver, private InputListener {
public:
	MarkerDriver(lv_obj_t* container);
	void onFrame(const DriveInfo& frame, Color* pixels) override;

	~MarkerDriver() override;

protected:
	void onStart() override;
	void onStop() override;


private:
	AutoControls autoControls;
	MarkerActionElement markerElement;
	BatsZoomElement batsElement;
	lv_obj_t* lockElement;

	bool motorsLock = false;
	void buttonPressed(uint i) override;
};


#endif //BATCONTROLLER_FIRMWARE_MARKERDRIVER_H
