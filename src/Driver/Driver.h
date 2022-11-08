#ifndef BATCONTROLLER_FIRMWARE_DRIVER_H
#define BATCONTROLLER_FIRMWARE_DRIVER_H

#include <DriveInfo.h>
#include <Display/Color.h>
#include "../Interface/LVObject.h"

class Driver{
public:
	void start();
	void stop();

	virtual void onFrame(const DriveInfo& frame, Color* pixels);

protected:
	virtual void onStart(){};
	virtual void onStop(){};
};


#endif //BATCONTROLLER_FIRMWARE_DRIVER_H
