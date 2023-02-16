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

class DriveScreen : public LVScreen, private InputListener, private DisconnectListener, private LoopListener, private ComListener {
public:
	DriveScreen(DriveMode mode, std::unique_ptr<Driver> customDriver = nullptr);
	~DriveScreen() override;

	void onStarting() override;
	void onStart() override;
	void onStop() override;
    void setInfoElement(std::unique_ptr<GeneralInfoElement> infoElement);


private:
	void onDisconnected() override;
	void onCameraError(uint8_t errorCode) override;

	void buttonReleased(uint i) override;
	void buttonPressed(uint i) override;

	void loop(uint micros) override;

	void setMode(DriveMode mode);
	DriveMode currentMode = DriveMode::Idle;
	std::unique_ptr<Driver> driver;

	Feed feed;
	lv_obj_t* driverLayer;

	lv_obj_t* img;
	lv_img_dsc_t imgDsc{};
	Color* imgBuf = nullptr;

	std::unique_ptr<GeneralInfoElement> infoElement;

	OverrideElement overrideElement;

	DriveMode originalMode = DriveMode::Idle;
	void hideOverrideElement();
	void showOverrideElement();

	static constexpr uint32_t OverrideDuration = 3000; // [ms]
	static constexpr uint32_t OverrideShowDuration = 600; // [ms]
	uint32_t overrideTime = 0;
	bool overrideShown = false;
	bool overrideDone = false;

	void toMenu();
};


#endif //BATCONTROLLER_FIRMWARE_DRIVESCREEN_H
