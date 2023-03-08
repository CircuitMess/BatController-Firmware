#ifndef BATCONTROLLER_FIRMWARE_INPUTPS_H
#define BATCONTROLLER_FIRMWARE_INPUTPS_H

#include <core/lv_obj.h>
#include <functional>
#include <string>

class InputPS {
public:
	InputPS(lv_obj_t* obj, lv_group_t* inputGroup);
	~InputPS();

	void start();
	void stop();
	void setCallbackDone(std::function<void(std::string)> cb);
	void setCallbackBack(std::function<void()> cb);

	void setPassword(const char* password);

private:
	void toPassword();

	lv_obj_t* input;
	lv_obj_t* kb;
	lv_obj_t* title;
	lv_obj_t* taPassword;

	std::function<void(std::string pass)> callbackDone = nullptr;
	std::function<void()> callbackBack = nullptr;
	lv_group_t* inputGroup;

	const char* pass = nullptr;
};


#endif //BATCONTROLLER_FIRMWARE_INPUTPS_H
