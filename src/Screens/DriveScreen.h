#ifndef BATCONTROLLER_FIRMWARE_DRIVESCREEN_H
#define BATCONTROLLER_FIRMWARE_DRIVESCREEN_H

#include "../Driver/Driver.h"
#include "../Interface/LVScreen.h"
#include <memory>
#include <Input/InputListener.h>
#include <DriveMode.h>
#include <Loop/LoopListener.h>
#include <array>
#include <Display/Color.h>
#include <DisconnectListener.h>
#include "../Elements/GeneralInfoElement.h"

class Feed {

};

class DriveScreen : public LVScreen, private InputListener, private LoopListener, private DisconnectListener {
public:
	DriveScreen(DriveMode mode);
	void onStart() override;
	void onStop() override;

private:
	void onDisconnected() override;

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	void loop(uint micros) override;

	void setMode(DriveMode mode);
	DriveMode currentMode = DriveMode::Idle;
	std::unique_ptr<Driver> driver;

	Feed feed;
	//FeedDisplay
	std::unique_ptr<Driver> driver;
	lv_obj_t* topLayer;

	lv_obj_t* canvas;
	std::array<Color, 160 * 120> canvasBuf;

	GeneralInfoElement infoElement;
};


#endif //BATCONTROLLER_FIRMWARE_DRIVESCREEN_H
