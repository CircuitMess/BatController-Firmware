#ifndef BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H
#define BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H

#include <Input/InputListener.h>
#include <DisconnectListener.h>
#include "../../Interface/LVScreen.h"
#include "Storage.h"

class SimpleProgScreen : public LVScreen, private DisconnectListener {
public:
	SimpleProgScreen();

	void onStart() override;
	void onStop() override;

private:
	void onDisconnected() override;
	Simple::Storage storage;
};


#endif //BATCONTROLLER_FIRMWARE_SIMPLEPROGSCREEN_H
