#ifndef BATCONTROLLER_FIRMWARE_DRIVER_H
#define BATCONTROLLER_FIRMWARE_DRIVER_H

#include "../Interface/LVObject.h"

class Feed; //TODO - maknuti, koristiti pravi Feed

class Driver{
public:
	void start();
	void stop();

protected:
	virtual void onStart(){};
	virtual void onStop(){};
};


#endif //BATCONTROLLER_FIRMWARE_DRIVER_H
