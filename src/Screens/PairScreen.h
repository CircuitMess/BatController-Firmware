#ifndef BATCONTROLLER_FIRMWARE_PAIRSCREEN_H
#define BATCONTROLLER_FIRMWARE_PAIRSCREEN_H

#include <Display/Sprite.h>
#include "../Interface/LVScreen.h"
#include "../PairService.h"
#include "PairScreenElements/ScanAruco.h"
#include "PairScreenElements/Connecting.h"
#include "PairScreenElements/Error.h"
#include "PairScreenElements/ScanQR.h"
#include "PairScreenElements/InputPS.h"


class PairScreen : public LVScreen {
public:
	PairScreen(bool disconnect = false);
	~PairScreen() override;

	void onStart() override;
	void onStop() override;
	void onStarting() override;

private:

	ScanAruco scanAruco;
	Connecting connecting;
	Error error;
	ScanQR scanQR;
	InputPS input;

	PairService pair;

	std::string ssid;
	std::string password;
	bool disconnect = false;

	uint16_t randID;
	char directSSID[14];
	char directPass[10];

	void resetDirect();

};


#endif //BATCONTROLLER_FIRMWARE_PAIRSCREEN_H

