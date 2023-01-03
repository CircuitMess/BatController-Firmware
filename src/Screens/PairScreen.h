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
#include "../WiFiService.h"


class PairScreen : public LVScreen, private LoopListener {
public:
	PairScreen();
	~PairScreen() override;

	void onStart() override;
	void onStop() override;

private:
	void loop(uint micros) override;

	ScanAruco scanAruco;
	Connecting connecting;
	Error error;
	ScanQR scanQR;
	InputPS input;

	PairService pair;
	WiFiService wifi;

	std::string ssid;
	std::string password;

	uint16_t randID;

	uint32_t microCounter;
	static constexpr uint32_t timeout = 5000000;
};


#endif //BATCONTROLLER_FIRMWARE_PAIRSCREEN_H

