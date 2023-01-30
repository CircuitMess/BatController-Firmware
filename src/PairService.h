
#ifndef BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
#define BATCONTROLLER_FIRMWARE_PAIRSERVICE_H


#include <Arduino.h>
#include <AsyncTCP.h>
#include <Loop/LoopListener.h>
#include <memory>
#include "WiFiService.h"

enum class PairError : uint8_t {
	PairOk = 0, ExternalWiFiConnected, ExternalWiFiTimeout, ServerError
};

class PairService : private LoopListener {
public:
	PairService();
	~PairService();

	/**
	 * Start looking for clients attempting to connect.
	 * Starts a WiFi AP if directConnection is true, otherwise attempts to connect to specified network in STA mode.
	 *
	 * @param directConnection True - start a WiFi AP with ssid and pass, False - attempt to connect to a network with ssid and pass
	 */
	void start(const char* wifiConnected, const char* pass, bool directConnection = true);
	void stop();

	void setDoneCallback(std::function<void(PairError)> doneCallback);


private:
	std::unique_ptr<AsyncClient> client;

	std::function<void(PairError)> doneCallback;

	void loop(uint micros) override;

	AsyncServer* server = nullptr;

	WiFiService wifi;

	bool directConnection = true;

};


#endif //BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
