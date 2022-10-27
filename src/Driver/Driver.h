#ifndef BATCONTROLLER_FIRMWARE_DRIVER_H
#define BATCONTROLLER_FIRMWARE_DRIVER_H

#include "../Interface/LVObject.h"

class Feed; //TODO - maknuti, koristiti pravi Feed

class Driver{
public:
	Driver(Feed& feed, lv_obj_t* elementContainer);
	void start();
	void stop();

protected:
	virtual void onStart(){};
	virtual void onStop(){};

	Feed& feed;
	lv_obj_t* elementContainer;
};


#endif //BATCONTROLLER_FIRMWARE_DRIVER_H
