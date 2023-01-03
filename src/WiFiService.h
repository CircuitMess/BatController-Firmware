#ifndef BATCONTROLLER_FIRMWARE_WIFISERVICE_H
#define BATCONTROLLER_FIRMWARE_WIFISERVICE_H

#include <memory>
#include <functional>
#include <Loop/LoopListener.h>

class WiFiService : LoopListener {
public:
	WiFiService();
	~WiFiService() override = default;

	void start();
	void stop();

	void setDoneCallback(std::function<void()> cb);
	IPAddress getIPAddress();

private:
	void loop(uint micros) override;

	std::function<void()> doneCallback;
};


#endif //BATCONTROLLER_FIRMWARE_WIFISERVICE_H
