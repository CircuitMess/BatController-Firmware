#ifndef BATCONTROLLER_FIRMWARE_PROGEDITSCREEN_H
#define BATCONTROLLER_FIRMWARE_PROGEDITSCREEN_H

#include <DisconnectListener.h>
#include <Input/InputListener.h>
#include "../../Interface/LVScreen.h"
#include "SimpleProgModel.h"
#include "ActionEditModal.h"
#include <functional>

class ProgEditScreen : public LVScreen, private DisconnectListener, private InputListener{
public:
	ProgEditScreen(Simple::Program& program, std::function<void()> saveCallback);
	void onStart() override;
	void onStop() override;
private:
	void buttonReleased(uint i) override;
	Simple::Program& program;

	std::function<void()> saveCallback;

	ActionEditModal modal;
};


#endif //BATCONTROLLER_FIRMWARE_PROGEDITSCREEN_H
