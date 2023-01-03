#ifndef BATCONTROLLER_FIRMWARE_SCANQR_H
#define BATCONTROLLER_FIRMWARE_SCANQR_H

#include <core/lv_obj.h>
#include <WString.h>
#include <functional>
#include <string>
#include <IPAddress.h>

class ScanQR {
public:
	ScanQR(lv_obj_t* obj, lv_group_t* inputGroup);
	~ScanQR();

	void start(std::string ssid, std::string password, IPAddress ipAddress);
	void stop();
	void setCallback(std::function<void()> cb);

private:
	lv_obj_t* scanQR;
	lv_obj_t* qr;
	lv_obj_t* text;
	lv_obj_t* title;

	static constexpr uint8_t Scale = 10;
	static constexpr uint8_t Multiplier = 8;

	std::function<void()> callback = nullptr;
	lv_group_t* inputGroup;
	char* data;
};


#endif //BATCONTROLLER_FIRMWARE_SCANQR_H
