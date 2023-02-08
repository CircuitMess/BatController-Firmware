#ifndef BATCONTROLLER_FIRMWARE_PROGEDITSCREEN_H
#define BATCONTROLLER_FIRMWARE_PROGEDITSCREEN_H

#include <DisconnectListener.h>
#include <Input/InputListener.h>
#include "../../Interface/LVScreen.h"
#include "SimpleProgModel.h"
#include "ActionEditModal.h"
#include "ActionPickModal.h"
#include "../../Elements/GeneralInfoElement.h"
#include <functional>

class ProgEditScreen : public LVScreen, private DisconnectListener, private InputListener{
public:
	ProgEditScreen(const Simple::Program& program, std::function<void(Simple::Program)> saveCallback);
	void onStart() override;
	void onStop() override;

private:
	Simple::Program program;
	void addAction(Simple::Action& action);
	void addNewActionButton();
	void buttonReleased(uint i) override;
	void buttonPressed(uint i) override;

	std::function<void(Simple::Program)> saveCallback;

	ActionEditModal editModal;
	ActionPickModal pickModal;

	GeneralInfoElement infoElement;
	lv_obj_t* newAction;
	lv_obj_t* actionView;
	lv_obj_t* footer;

	static constexpr uint32_t holdTime = 800; //0.8s hold to confirm erase
	static constexpr uint8_t rowLength = 7;
	lv_timer_t* progDeleteTimer;
	uint32_t holdStartTime = 0;
};


#endif //BATCONTROLLER_FIRMWARE_PROGEDITSCREEN_H
