#ifndef BATCONTROLLER_FIRMWARE_LOWBATTERYSERVICE_H
#define BATCONTROLLER_FIRMWARE_LOWBATTERYSERVICE_H

#include <Loop/LoopListener.h>
#include <Com/ComListener.h>
#include "Elements/LowBatteryModal.h"

class LowBatteryService : private LoopListener, ComListener {
public:
	void begin();
	void end();

private:
    void loop(uint micros) override;
    void onBattery(uint8_t percent, bool charging) override;
	void checkCtrlBattery();

    LowBatteryModal* modal = nullptr;

    BatType shutdownType;
    bool modalShowing = false;
    uint32_t shutdownTimer = 0;
    uint32_t checkTimer = 0;
    static constexpr uint32_t CheckInterval = 3000000;
    static constexpr uint32_t ShutdownDelay = 4000000;

	void showModal(BatType type);
	void stopModal();
};

extern LowBatteryService BatteryShutdown;

#endif //BATCONTROLLER_FIRMWARE_LOWBATTERYSERVICE_H
