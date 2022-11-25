#ifndef BATCONTROLLER_FIRMWARE_MARKERDRIVER_H
#define BATCONTROLLER_FIRMWARE_MARKERDRIVER_H


#include "Driver.h"
#include "AutoControls.h"
#include "../Elements/MarkerActionElement.h"

class MarkerDriver : public Driver {
public:
	MarkerDriver(lv_obj_t* container, LVScreen* screen);
	void onFrame(const DriveInfo& frame, Color* pixels) override;

    ~MarkerDriver() override;

protected:
	void onStart() override;
	void onStop() override;


private:
	AutoControls autoControls;
	MarkerActionElement markerElement;
};


#endif //BATCONTROLLER_FIRMWARE_MARKERDRIVER_H
