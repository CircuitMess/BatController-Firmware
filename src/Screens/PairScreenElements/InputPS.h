
#ifndef BATCONTROLLER_FIRMWARE_INPUTPS_H
#define BATCONTROLLER_FIRMWARE_INPUTPS_H

#include <core/lv_obj.h>
#include <functional>
#include <string>

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

    std::function<void()> callbackDone = nullptr;
    std::function<void()> callbackBack = nullptr;
    lv_group_t *inputGroup;
};


#endif //BATCONTROLLER_FIRMWARE_INPUTPS_H
