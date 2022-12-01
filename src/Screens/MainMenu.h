#ifndef BATCONTROLLER_FIRMWARE_MAINMENU_H
#define BATCONTROLLER_FIRMWARE_MAINMENU_H

#include <Input/InputListener.h>
#include "../Interface/LVScreen.h"
#include "../Elements/GeneralInfoElement.h"



class MainMenu: public LVScreen, private InputListener{
public:
    MainMenu();
    ~MainMenu() override;
    void onStarting() override;
    void onStart() override;
    void onStop() override;

private:
    struct Item {
        const char* icon;
        int16_t offset;
    } static const Items[];
    static const uint8_t ItemCount;

    lv_obj_t* top;
    lv_obj_t* right;
    lv_obj_t* mid;

    std::vector<lv_obj_t*> bigContainers;
    std::vector<lv_obj_t*> bigs;
    std::vector<lv_obj_t*> bigLabels;
    std::vector<lv_obj_t*> smalls;

    void loadGIFs();
    void unloadGIFs();

    void scrollTo(uint8_t index);
    void launch();
    void selectNext();
    void selectPrev();

    void setRed(uint8_t index, bool reverse = false);

    uint8_t selected = 0;
    bool inited = false;

    GeneralInfoElement* infoElement = nullptr;

    void buttonPressed(uint i) override;
};


#endif //BATCONTROLLER_FIRMWARE_MAINMENU_H
