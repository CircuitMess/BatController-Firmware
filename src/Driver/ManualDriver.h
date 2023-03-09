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

	void loop(uint micros) override;

	BoostElement* boost;
	uint8_t boostGauge = 100;
	uint8_t boostGaugeStart = 100;
	bool boostActive = false;
	uint32_t boostTimer = 0;
	bool boostPressed = false;
	static constexpr uint boostFillDuration = 4500000; //recharging boost from empty to full requires 4.5s
	static constexpr uint boostDuration = 3500000; //using boost bar from full to empty lasts 3.5s

	void sendDriveDir();
	uint8_t lastDir = 0;


	uint8_t getGyroDir() const;
	uint8_t getGyroSpeed() const;
	static constexpr float GyroRange = 16384.0f; //14-bit precision
	static constexpr float GyroDeadzone = 0.22f;
	static constexpr uint8_t GyroSpeedRange = 15;
	void sendGyro();
};


#endif //BATCONTROLLER_FIRMWARE_MANUALDRIVER_H
