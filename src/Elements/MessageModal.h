#ifndef BATCONTROLLER_FIRMWARE_MESSAGEMODAL_H
#define BATCONTROLLER_FIRMWARE_MESSAGEMODAL_H


#include <Input/InputListener.h>
#include "../Interface/LVModal.h"

class MessageModal : public LVModal, private InputListener {
public:
	MessageModal(LVScreen* parent, const char* message, uint32_t timeout = UINT32_MAX);
protected:
	void onStop() override;
	void onStart() override;
private:
	void buttonReleased(uint i) override;
private:
	lv_timer_t* timer = nullptr;
};


#endif //BATCONTROLLER_FIRMWARE_MESSAGEMODAL_H
