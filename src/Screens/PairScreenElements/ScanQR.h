
#ifndef BATCONTROLLER_FIRMWARE_SCANQR_H
#define BATCONTROLLER_FIRMWARE_SCANQR_H

#include <core/lv_obj.h>
#include <WString.h>
#include <functional>

class ScanQR {
public:
    ScanQR(lv_obj_t *obj, lv_group_t *inputGroup);
    ~ScanQR();

    void start(String networkName);
    void stop();
    void setCallback(std::function<void()> cb);

private:
    lv_obj_t* scanQR;
    lv_obj_t* canvas;
    lv_obj_t* text;
    lv_obj_t* title;

    lv_color_t* buffer;
    static constexpr uint8_t Scale = 10;
    static constexpr uint8_t Multiplier = 8;

    std::function<void()> callback = nullptr;
    lv_group_t *inputGroup;
};


#endif //BATCONTROLLER_FIRMWARE_SCANQR_H
