#ifndef BATCONTROLLER_FIRMWARE_SIGNALELEMENT_H
#define BATCONTROLLER_FIRMWARE_SIGNALELEMENT_H

#include <Com/ComListener.h>
#include "../Interface/LVObject.h"

class SignalElement : public LVObject, private ComListener {
public:
	SignalElement(lv_obj_t* parent);
	~SignalElement() override;
private:
	void onSignalStrength(uint8_t percent) override;
private:
	/**
	 * @param index = [0-3]
	 * 0->no signal
	 * 3->strong signal
	 */
	void setLevel(uint8_t index);
	uint8_t signalStrength;
	lv_obj_t* img;
};

#endif //BATCONTROLLER_FIRMWARE_SIGNALELEMENT_H