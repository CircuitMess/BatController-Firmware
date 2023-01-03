#ifndef BATCONTROLLER_FIRMWARE_ERROR_H
#define BATCONTROLLER_FIRMWARE_ERROR_H

#include <core/lv_obj.h>
#include <functional>
#include <string>

class Error {
public:
	Error(lv_obj_t* obj, lv_group_t* inputGroup);
	~Error();

	void start(std::string errorMessage = "Connection broke press A to go back to input screen");
	void stop();
	void setCallback(std::function<void()> cb);

private:
	lv_obj_t* error;
	lv_obj_t* img;
	lv_obj_t* text;

	std::function<void()> callback = nullptr;
	lv_group_t* inputGroup;
};


#endif //BATCONTROLLER_FIRMWARE_ERROR_H
