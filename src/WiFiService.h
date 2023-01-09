#ifndef BATCONTROLLER_FIRMWARE_WIFISERVICE_H
#define BATCONTROLLER_FIRMWARE_WIFISERVICE_H

#include <memory>
#include <functional>
#include <Loop/LoopListener.h>

class WiFiService : private LoopListener {
public:
	WiFiService();
	~WiFiService() override = default;

	void start(const char* ssid, const char* pass);
	void stop();

	void setDoneCallback(std::function<void(bool)> cb);
	static IPAddress getIPAddress();
	static bool isConnected();

private:
	void loop(uint micros) override;

	uint32_t retryCounter = 0;
	uint8_t retryCount = 0;

	const char* ssid;
	const char* pass;

	static constexpr uint32_t RetryInterval = 3500000;
	static constexpr uint8_t RetryTries = 3;

	std::function<void(bool)> doneCallback;
};


#endif //BATCONTROLLER_FIRMWARE_WIFISERVICE_H
