#ifndef BATCONTROLLER_FIRMWARE_MAINMENU_H
#define BATCONTROLLER_FIRMWARE_MAINMENU_H

#include <Input/InputListener.h>
#include <memory>
#include <DisconnectListener.h>
#include "../Interface/LVScreen.h"
#include "../Elements/GeneralInfoElement.h"



class MainMenu: public LVScreen, private InputListener,  private DisconnectListener{
public:
    MainMenu();
    void onStarting() override;
    void onStart() override;
    void onStop() override;
    void setInfoElement(std::unique_ptr<GeneralInfoElement> infoElement);
	static void resetLastSelected();
private:
	static constexpr uint8_t ItemCount = 6;
	const char* const Items[ItemCount] = {
			"Manual",
			"SimpleProg",
			"Dance",
			"Ball",
			"Marker",
			"Settings",
	};

    lv_obj_t* top;
    lv_obj_t* right;
	lv_obj_t* midContainer;
    lv_obj_t* mid;

    std::vector<lv_obj_t*> bigContainers;
    std::vector<lv_obj_t*> bigs;
    std::vector<lv_obj_t*> bigLabels;
    std::vector<lv_obj_t*> smalls;

    void loadGIFs();
    void unloadGIFs();
    void unloadLastGIF();

    void scrollTo(uint8_t index);
    void launch();
    void selectNext();
    void selectPrev();

    void setRed(uint8_t index, bool reverse = false);

	void outro();
	void clearInput();
	bool launching = false;

    uint8_t selected = 0;
	static uint8_t lastSelected;

    std::unique_ptr<GeneralInfoElement> infoElement;

    void buttonPressed(uint i) override;

	void onDisconnected() override;
};


#endif //BATCONTROLLER_FIRMWARE_MAINMENU_H
