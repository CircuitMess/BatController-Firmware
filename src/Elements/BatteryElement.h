#ifndef BATCONTROLLER_FIRMWARE_BATTERYELEMENT_H
#define BATCONTROLLER_FIRMWARE_BATTERYELEMENT_H

#include <Loop/LoopListener.h>
#include <LVObject.h>

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
	void setCharging(bool charging);
	void setBlinking(bool blinking);

private:
	void loop(uint micros) override;

	lv_obj_t* img;
	lv_obj_t* device;

	bool blinking = false;
	bool hidden = false;
	uint32_t blinkTimer = 0;
	static constexpr uint32_t BlinkInterval = 500; // [ms]

	bool charging = false;
	uint8_t picIndex = 0;
	uint32_t chargeTimer = 0;
	static const uint32_t ChargeInterval = 500000;
};


#endif //BATCONTROLLER_FIRMWARE_BATTERYELEMENT_H
