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
#include <memory>

class ProgEditScreen : public LVScreen, private DisconnectListener, private InputListener {
public:
	ProgEditScreen(const Simple::Program& program, std::function<void(Simple::Program)> saveCallback);
	~ProgEditScreen() override;

	void onStarting() override;
	void onStart() override;
	void onStop() override;

	void setInfoElement(std::unique_ptr<GeneralInfoElement> infoElement);


private:
	Simple::Program program;
	void addAction(const Simple::Action& action);
	void addNewActionButton();
	void buttonReleased(uint i) override;
	void buttonPressed(uint i) override;

	void onDisconnected() override;
	std::function<void(Simple::Program)> saveCallback;

	ActionEditModal editModal;
	ActionPickModal pickModal;

	std::unique_ptr<GeneralInfoElement> infoElement;

	lv_obj_t* newAction;
	lv_obj_t* actionView;
	lv_obj_t* footer;

	static constexpr uint32_t holdTime = 800; //0.8s hold to confirm erase
	static constexpr uint8_t rowLength = 7;
	lv_timer_t* progDeleteTimer;
};


#endif //BATCONTROLLER_FIRMWARE_PROGEDITSCREEN_H
