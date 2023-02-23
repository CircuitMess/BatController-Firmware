#ifndef BATCONTROLLER_FIRMWARE_MESSAGEMODAL_H
#define BATCONTROLLER_FIRMWARE_MESSAGEMODAL_H


#include <Input/InputListener.h>
#include "../Interface/LVModal.h"
#include <functional>

class MessageModal : public LVModal, private InputListener {
public:
	MessageModal(LVScreen* parent, const char* message, uint32_t timeout = UINT32_MAX);

	void setDismissCallback(std::function<void()> dismissCallback);

protected:
	void onStop() override;
	void onStart() override;

private:
	void buttonReleased(uint i) override;
	lv_timer_t* timer = nullptr;

	std::function<void()> dismissCallback;

	void dismiss();
};


#endif //BATCONTROLLER_FIRMWARE_MESSAGEMODAL_H
