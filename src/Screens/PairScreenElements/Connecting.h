
#ifndef BATCONTROLLER_FIRMWARE_CONNECTING_H
#define BATCONTROLLER_FIRMWARE_CONNECTING_H


#include <core/lv_obj.h>

class Connecting {
public:
    Connecting(lv_obj_t* obj);
    ~Connecting();

    void start();
    void stop();

private:
    lv_obj_t* connecting;
    lv_obj_t* gif;
    lv_obj_t* text;

};


#endif //BATCONTROLLER_FIRMWARE_CONNECTING_H
