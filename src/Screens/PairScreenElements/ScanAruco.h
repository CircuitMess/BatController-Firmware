
#ifndef BATCONTROLLER_FIRMWARE_SCANARUCO_H
#define BATCONTROLLER_FIRMWARE_SCANARUCO_H

#include <functional>

class ScanAruco {
public:
    ScanAruco(lv_obj_t *obj, lv_group_t *inputGroup);
    virtual ~ScanAruco();

    void start(uint16_t id);
    void stop();
    void setCallback(std::function<void()> cb);

private:
    lv_obj_t* scanAruco;
    lv_obj_t* canvas;
    lv_obj_t* text;

    lv_color_t* buffer;
    lv_color_t black;
    lv_color_t white;

    std::function<void()> callback = nullptr;
    lv_group_t *inputGroup;

    static constexpr uint8_t Scale = 10;
    static constexpr uint8_t Multiplier = 8;
};


#endif //BATCONTROLLER_FIRMWARE_SCANARUCO_H
