
#ifndef BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
#define BATCONTROLLER_FIRMWARE_PAIRSERVICE_H


#include <Arduino.h>
#include <AsyncTCP.h>
#include <Loop/LoopListener.h>
#include <memory>

class PairService : private LoopListener {
public:
	PairService();
	~PairService();

	void start(uint16_t id);
	void stop();

	void setDoneCallback(std::function<void()> doneCallback);


private:
	char ssid[14];
	char password[10];

	std::unique_ptr<AsyncClient> client;

	std::function<void()> doneCallback;

	void loop(uint micros) override;

	AsyncServer* server = nullptr;

};


#endif //BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
