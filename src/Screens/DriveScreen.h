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
#include "../Feed/Feed.h"
#include "../Elements/OverrideElement.h"

class DriveScreen : public LVScreen, private InputListener, private DisconnectListener, private LoopListener {
public:
	DriveScreen(DriveMode mode);
	virtual ~DriveScreen();

	void onStarting() override;
	void onStart() override;
	void onStop() override;

private:
	void onDisconnected() override;
	void buttonReleased(uint i) override;
	void buttonPressed(uint i) override;
	void loop(uint micros) override;

	void setMode(DriveMode mode);
	void hideOverrideElement();

	DriveMode currentMode = DriveMode::Idle;
	std::unique_ptr<Driver> driver;

	Feed feed;
	lv_obj_t* driverLayer;

	lv_obj_t* img;
	lv_img_dsc_t imgDsc {};
	Color* imgBuf = nullptr;

	GeneralInfoElement infoElement;
	OverrideElement overrideElement;

	uint8_t fillPercent = 0;
	uint16_t microCounter = 0;
	static const uint16_t fillInterval = 5000;
};


#endif //BATCONTROLLER_FIRMWARE_DRIVESCREEN_H
