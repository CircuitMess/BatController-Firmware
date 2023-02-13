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

	void onStarting() override;
	void onStart() override;
	void onStop() override;

	static void touchIndex();

private:
	void onDisconnected() override;
	Simple::Storage storage;
	void buildProgView();

	static uint8_t lastProgramIndex;

	GeneralInfoElement infoElement;
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

	lv_timer_t* progDeleteTimer;
	uint32_t holdStartTime = 0;

};


#endif //BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H
