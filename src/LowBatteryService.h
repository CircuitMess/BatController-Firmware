#ifndef BATCONTROLLER_FIRMWARE_LOWBATTERYSERVICE_H
#define BATCONTROLLER_FIRMWARE_LOWBATTERYSERVICE_H

#include <Loop/LoopListener.h>
#include <Com/ComListener.h>
#include "Elements/LowBatteryModal.h"

class LowBatteryService : private LoopListener, ComListener {
public:
    LowBatteryService();
    ~LowBatteryService();


private:
    void loop(uint micros) override;
    void onBattery(uint8_t percent, bool charging) override;

    LowBatteryModal* batModal = nullptr;

    BatType shutdownBattery;
    bool modalShowing = false;
    uint32_t shutdownTimer = 0;
    uint32_t timer = 0;
    static constexpr uint32_t checkInterval = 5000000;
    static constexpr uint32_t shutdownDelay = 3000000;
};


#endif //BATCONTROLLER_FIRMWARE_LOWBATTERYSERVICE_H
