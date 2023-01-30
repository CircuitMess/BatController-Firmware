#ifndef BATCONTROLLER_FIRMWARE_MESSAGEMODAL_H
#define BATCONTROLLER_FIRMWARE_MESSAGEMODAL_H


#include "../Interface/LVModal.h"

class MessageModal : public LVModal {
public:
	MessageModal(LVScreen* parent,  const char* message);
	~MessageModal() override;

protected:
	void onStart() override;
	void onStop() override;

};


#endif //BATCONTROLLER_FIRMWARE_MESSAGEMODAL_H
