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
	static constexpr size_t directionSendInterval = 100000; //send direction every 100ms
	const uint directions[4] = { BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT };

	void loop(uint micros) override;

	BoostElement* boost;
	uint8_t boostGauge = 100;
	bool boostActive = false;
	uint32_t boostTimer = 0;
	bool boostPressed = false;
	static constexpr uint boostFillRate = 40000; //increment boost by 1 every 40ms when recharging boost
	static constexpr uint boostConsumptionRate = 20000; //decrease by 1 every 10ms when using boost

	void sendDriveDir();

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
