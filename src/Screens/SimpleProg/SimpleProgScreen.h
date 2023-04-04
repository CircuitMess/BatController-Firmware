#ifndef BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H
#define BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H

#include <Input/InputListener.h>
#include <DisconnectListener.h>
#include <memory>
#include <LVScreen.h>
#include "Storage.h"
#include "../../Elements/GeneralInfoElement.h"

class SimpleProgScreen : public LVScreen, private DisconnectListener, private InputListener {
public:
	SimpleProgScreen();
	~SimpleProgScreen() override;

	void onStarting() override;
	void onStart() override;
	void onStop() override;

	void setInfoElement(std::unique_ptr<GeneralInfoElement> infoElement);

	static void touchIndex();

private:
	void onDisconnected() override;
	Simple::Storage storage;
	void buildProgView();

	static uint8_t lastProgramIndex;

	std::unique_ptr<GeneralInfoElement> infoElement;

	lv_obj_t* progView;
	lv_obj_t* footer;
	lv_obj_t* newProg;

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	void startEdit(uint8_t index);

	static constexpr uint8_t programWidth = 141;
	static constexpr uint8_t programHeight = 17;
	static constexpr uint8_t spaceBetweenProgs = 3;
	static constexpr uint32_t holdTime = 1000; //1s hold to confirm play/eras

	static constexpr uint32_t clickTimeMax = 350; //press and release under 350ms is tolerated as a click
	uint32_t backClickTimer = 0;

	lv_timer_t* progDeleteTimer;
	uint32_t holdStartTime = 0;

	void startDrive(uint8_t index);

};


#endif //BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H
