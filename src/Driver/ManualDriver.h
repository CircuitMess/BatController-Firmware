#ifndef BATCONTROLLER_FIRMWARE_MANUALDRIVER_H
#define BATCONTROLLER_FIRMWARE_MANUALDRIVER_H

#include "Driver.h"
#include "../Elements/BoostElement.h"
#include <Loop/LoopListener.h>
#include <Input/InputListener.h>

class ManualDriver : public Driver, private LoopListener, private InputListener {
public:
	ManualDriver(lv_obj_t* elementContainer);

    ~ManualDriver() override;

protected:
	void onStart() override;
	void onStop() override;

private:
	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;
	uint8_t dir = 0;
	size_t directionSendTimer = 0;
	static constexpr size_t directionSendInterval = 100000; //send direction every 100ms

	void loop(uint micros) override;

	BoostElement boost;
	uint8_t boostGauge = 0;
	bool boostActive = false;
	uint32_t boostTimer = 0;
	static constexpr uint boostFillRate = 40000; //increment boost by 1 every 40ms when recharging boost
	static constexpr uint boostConsumptionRate = 20000; //decrease by 1 every 10ms when using boost
};


#endif //BATCONTROLLER_FIRMWARE_MANUALDRIVER_H
