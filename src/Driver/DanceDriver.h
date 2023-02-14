#ifndef BATCONTROLLER_FIRMWARE_DANCEDRIVER_H
#define BATCONTROLLER_FIRMWARE_DANCEDRIVER_H


#include <Input/InputListener.h>
#include "Driver.h"
#include "../Elements/DanceElement.h"

class DanceDriver : public Driver, private InputListener {
public:
	DanceDriver(lv_obj_t* elementContainer);
	~DanceDriver() override;

protected:
	void onStart() override;
	void onStop() override;

private:
	void buttonReleased(uint i) override;
	void buttonPressed(uint i) override;

	DanceElement danceElement;
	uint8_t danceIndex = 0;
};


#endif //BATCONTROLLER_FIRMWARE_DANCEDRIVER_H
