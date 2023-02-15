#ifndef BATCONTROLLER_FIRMWARE_DANCEELEMENT_H
#define BATCONTROLLER_FIRMWARE_DANCEELEMENT_H


#include "../Interface/LVObject.h"
#include <DanceType.h>
#include <string>

class DanceElement : public LVObject{
public:
    DanceElement(lv_obj_t *parent);

    void setCurrentDance(DanceType danceType);
    void arrowLeftPressed(bool pressed);
    void arrowRightPressed(bool pressed);

private:
    lv_obj_t* arrowL;
    lv_obj_t* arrowR;
    lv_obj_t* currentDance;
    lv_obj_t* img;
    /**
     * make sure that dances align in order in DanceType.h (Bat-Common)
     */
    const std::string dances[6] = { "Idle", "Wiggle", "Step", "Spin", "BackItUp", "Waltz"};
};


#endif //BATCONTROLLER_FIRMWARE_DANCEELEMENT_H
