#ifndef BATCONTROLLER_FIRMWARE_PAIRSCREEN_H
#define BATCONTROLLER_FIRMWARE_PAIRSCREEN_H

#include <Display/Sprite.h>
#include "../Interface/LVScreen.h"
#include "../PairService.h"
//#include "PairScreenElements/ScanAruco.h"
//#include "PairScreenElements/Connecting.h"
//#include "PairScreenElements/Error.h"
//#include "PairScreenElements/ScanQR.h"
#include "PairScreenElements/InputPS.h"

class PairScreen : public LVScreen{
public:
	PairScreen();
	~PairScreen() override;

	void onStart() override;
	void onStop() override;

private:
//    ScanAruco scanAruco;
//    Connecting connecting;
//    Error error;
//    ScanQR scanQR;
    InputPS input;

	PairService pair;
};


#endif //BATCONTROLLER_FIRMWARE_PAIRSCREEN_H

