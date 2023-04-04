
#ifndef BATCONTROLLER_FIRMWARE_LOWBATTERYMODAL_H
#define BATCONTROLLER_FIRMWARE_LOWBATTERYMODAL_H

#include <LVModal.h>
#include "BatteryElement.h"

class LowBatteryModal : public LVModal{
public:
    LowBatteryModal(LVScreen *parent, BatType batType);

private:
    lv_obj_t* text;
    lv_obj_t* img;

    static constexpr size_t w = 79;
    static constexpr size_t h = 48;
};


#endif //BATCONTROLLER_FIRMWARE_LOWBATTERYMODAL_H
