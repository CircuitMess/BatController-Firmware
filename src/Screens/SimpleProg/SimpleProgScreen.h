#ifndef BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H
#define BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H

#include <Input/InputListener.h>
#include <DisconnectListener.h>
#include "../../Interface/LVScreen.h"
#include "Storage.h"
#include "../../Elements/GeneralInfoElement.h"

class SimpleProgScreen : public LVScreen, private DisconnectListener, private InputListener {
public:
	SimpleProgScreen();

	void onStart() override;
	void onStop() override;

private:
	void onDisconnected() override;
	Simple::Storage storage;

	Simple::Program currentProgram;
	static uint8_t lastProgramIndex;

	GeneralInfoElement infoElement;
	lv_obj_t* progView;
	lv_obj_t* footer;
	lv_obj_t* newProg;

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	static constexpr uint8_t programWidth = 141;
	static constexpr uint8_t programHeight = 17;
	static constexpr uint8_t spaceBetweenProgs = 3;
	static constexpr uint32_t holdTime = 1000; //1s hold to confirm play/eras

	lv_timer_t* progDeleteTimer;
	uint32_t holdStartTime = 0;
};


#endif //BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H
