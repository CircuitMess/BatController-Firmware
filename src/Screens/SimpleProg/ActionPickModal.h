#ifndef BATCONTROLLER_FIRMWARE_ACTIONPICKMODAL_H
#define BATCONTROLLER_FIRMWARE_ACTIONPICKMODAL_H

#include "../../Interface/LVModal.h"
#include "SimpleProgModel.h"
#include <functional>
#include <array>

class ActionPickModal : public LVModal {
public:
	explicit ActionPickModal(LVScreen* parent);
	void startPick(std::function<void(Simple::Action action)> pickedCallback);

protected:
	void onStart() override;
	void onStop() override;

private:
	void addAction(Simple::Action::Type type);

	static constexpr size_t w = 79;
	static constexpr size_t h = 64;
	static constexpr uint8_t rowLength = 3;
	std::function<void(Simple::Action action)> pickedCallback;

	static constexpr std::array<const char*, 6> actionNames = { "Drive", "Headlights", "Taillights", "Underlights", "Sound", "Delay" };

	lv_obj_t* actionLabel;
};


#endif //BATCONTROLLER_FIRMWARE_ACTIONPICKMODAL_H
