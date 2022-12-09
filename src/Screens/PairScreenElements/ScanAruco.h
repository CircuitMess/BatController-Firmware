
#ifndef BATCONTROLLER_FIRMWARE_SCANARUCO_H
#define BATCONTROLLER_FIRMWARE_SCANARUCO_H


class ScanAruco {
public:
    ScanAruco(lv_obj_t* obj);
    virtual ~ScanAruco();

    void start(uint16_t id);
    void stop();

private:
    lv_obj_t* scanAruco;
    lv_obj_t* canvas;
    lv_obj_t* text;

    lv_color_t* buffer;
    lv_color_t black;
    lv_color_t white;

    static constexpr uint8_t Scale = 10;
    static constexpr uint8_t Multiplier = 8;
};


#endif //BATCONTROLLER_FIRMWARE_SCANARUCO_H
