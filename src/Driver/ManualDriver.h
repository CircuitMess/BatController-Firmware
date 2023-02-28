#ifndef BATCONTROLLER_FIRMWARE_MANUALDRIVER_H
#define BATCONTROLLER_FIRMWARE_MANUALDRIVER_H

#include "Driver.h"
#include "../Elements/BoostElement.h"
#include <Loop/LoopListener.h>
#include <Input/InputListener.h>
#include <Pins.hpp>

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
	static constexpr size_t directionSendInterval = 1000000; //send direction every second
	const uint directions[4] = { BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT };

	void loop(uint micros) override;

	BoostElement* boost;
	uint8_t boostGauge = 100;
	uint8_t boostGaugeStart = 100;
	bool boostActive = false;
	uint32_t boostTimer = 0;
	bool boostPressed = false;
	static constexpr uint boostFillDuration = 4500000; //recharging boost from empty to full requires 4.5s
	static constexpr uint boostDuration = 3500000; //using boost bar from full to empty lasts 3.5s

	void sendDriveDir() const;
	uint8_t lastDir = 0;


	bool gyro = false;
	uint8_t gyroDir = 0;
	static constexpr uint8_t GyroAddr = 0x18;
	uint32_t gyroCounter = 0;
	static constexpr uint32_t GyroReadInterval = 10000;
	static constexpr uint32_t GyroCheckInterval = 100000;
	void sendGyro();
	void checkGyro();
};


#endif //BATCONTROLLER_FIRMWARE_MANUALDRIVER_H
