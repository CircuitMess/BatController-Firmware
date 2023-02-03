#ifndef BATCONTROLLER_FIRMWARE_ACTIONEDITMODAL_H
#define BATCONTROLLER_FIRMWARE_ACTIONEDITMODAL_H

#include "../../Interface/LVModal.h"
#include "SimpleProgModel.h"

class ActionEditModal : public LVModal {
public:
	explicit ActionEditModal(LVScreen* parent);
	void startEdit(Simple::Action& action);
protected:
	void onStart() override;
	void onStop() override;
private:


	static constexpr size_t w = 79;
	static constexpr size_t h = 48;

	static constexpr size_t entryW = 75;
	static constexpr size_t entryH = 13;

	void addDirection(Simple::Action& action);
	void addDuration(Simple::Action& action);
	void addSpeed(Simple::Action& action);
	void addToggle(Simple::Action& action);
	void addSound(Simple::Action& action);
	void addColor(Simple::Action& action);

};

#endif //BATCONTROLLER_FIRMWARE_ACTIONEDITMODAL_H
