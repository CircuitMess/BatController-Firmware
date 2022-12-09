
#ifndef BATCONTROLLER_FIRMWARE_INPUTPS_H
#define BATCONTROLLER_FIRMWARE_INPUTPS_H

#include <core/lv_obj.h>

class InputPS {
public:
    InputPS(lv_obj_t *obj, lv_group_t *inputGroup);
    ~InputPS();

    void start();
    void stop();

private:
    lv_obj_t* input;
    lv_obj_t* kb;
    lv_obj_t* taNetwork;
    lv_obj_t* taPassword;

};


#endif //BATCONTROLLER_FIRMWARE_INPUTPS_H
