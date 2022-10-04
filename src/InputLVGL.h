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
	lv_indev_t* getIndev();

private:
	uint32_t lastKey = -1;
	bool pressed = false;
	static std::map<uint8_t, lv_key_t> keyMap;

	lv_indev_t* inputDevice;
	static InputLVGL* instance;
};


#endif //BATCONTROLLER_FIRMWARE_INPUTLVGL_H
