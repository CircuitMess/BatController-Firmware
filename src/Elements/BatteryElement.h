#ifndef BATCONTROLLER_FIRMWARE_BATTERYELEMENT_H
#define BATCONTROLLER_FIRMWARE_BATTERYELEMENT_H

#include <Loop/LoopListener.h>
#include "../Interface/LVObject.h"

enum class BatType {
	Controller, Batmobile
};

class BatteryElement : public LVObject, private LoopListener {
public:
	BatteryElement(lv_obj_t* parent, BatType type);
	~BatteryElement() override;
	/**
	 * @param index = [0-7]
	 * 0->empty
	 * 7->full
	 */
	void setLevel(uint8_t index);

private:
	void loop(uint micros) override;

	lv_obj_t* img;
	lv_obj_t* device;
};


#endif //BATCONTROLLER_FIRMWARE_BATTERYELEMENT_H
