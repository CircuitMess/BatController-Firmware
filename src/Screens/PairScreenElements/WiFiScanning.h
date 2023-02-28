#ifndef BATCONTROLLER_FIRMWARE_WIFISCANNING_H
#define BATCONTROLLER_FIRMWARE_WIFISCANNING_H


#include <Loop/LoopListener.h>
#include <core/lv_obj.h>
#include <functional>

class WiFiScanning : private LoopListener{
public:
	WiFiScanning(lv_obj_t* obj, lv_group_t* inputGroup);
	virtual ~WiFiScanning();

	void start();
	void stop();

	void setCallbackBack(std::function<void()> cb);
	void setCallbackDone(std::function<void(std::string ssid)> cb);
	void setCallbackError(std::function<void(std::string errorMessage)> cb);

private:
	void loop(uint micros) override;
	void startList();
	void startScan();

	lv_obj_t* scanObj;
	lv_obj_t* gif;
	lv_obj_t* listObj;
	lv_obj_t* list;
	lv_group_t* inputGroup;

	uint8_t numOfNetworks = 0;
	uint32_t counter = 0;

	static constexpr uint32_t Timeout = 5000000;
	static constexpr uint8_t ElementWidth = 141;
	static constexpr uint8_t ElementHeight = 17;

	std::function<void()> callbackBack = nullptr;
	std::function<void(std::string ssid)> callbackDone = nullptr;
	std::function<void(std::string ssid)> callbackError = nullptr;
};


#endif //BATCONTROLLER_FIRMWARE_WIFISCANNING_H
