#ifndef BATCONTROLLER_FIRMWARE_INPUTLVGL_H
#define BATCONTROLLER_FIRMWARE_INPUTLVGL_H

#include <Input/InputListener.h>
#include <map>
#include <queue>
#include <lvgl.h>

class InputLVGL : public InputListener{
public:
	InputLVGL();
	void read(lv_indev_drv_t* drv, lv_indev_data_t* data);

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	static InputLVGL* getInstance();
	/**
	 * Enables navigation with up/down buttons. If disabled, buttons will be sent as up/down keys to objects.
	 * Enabled by default.
	 * @param enable true - vertical navigation enabled, false - vertical navigation disabled
	 */
	static void enableVerticalNavigation(bool enable);

	lv_indev_t* getIndev();

private:
	uint32_t lastKey = -1;
	bool pressed = false;
	static const std::map<uint8_t, lv_key_t> keyMap;

	static bool verticalNavigation;

	lv_indev_t* inputDevice;
	static InputLVGL* instance;
};


#endif //BATCONTROLLER_FIRMWARE_INPUTLVGL_H
