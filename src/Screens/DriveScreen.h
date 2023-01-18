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

class DriveScreen : public LVScreen, private InputListener, private DisconnectListener {
public:
	DriveScreen(DriveMode mode);
	virtual ~DriveScreen();

	void onStarting() override;
	void onStart() override;
	void onStop() override;
    void setInfoElement(std::unique_ptr<GeneralInfoElement> infoElement);


private:
	void onDisconnected() override;

	void buttonPressed(uint i) override;

	void setMode(DriveMode mode);
	DriveMode currentMode = DriveMode::Idle;
	std::unique_ptr<Driver> driver;

	Feed feed;
	lv_obj_t* driverLayer;

	lv_obj_t* img;
	lv_img_dsc_t imgDsc {};
	Color* imgBuf = nullptr;

	std::unique_ptr<GeneralInfoElement> infoElement;
};


#endif //BATCONTROLLER_FIRMWARE_DRIVESCREEN_H
