
#ifndef BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
#define BATCONTROLLER_FIRMWARE_PAIRSERVICE_H


#include <cstdint>
#include <Loop/LoopListener.h>
#include <WiFiClient.h>

class PairService {
public:
	PairService(uint16_t id);
	~PairService();

private:
	char ssid[14];
	char password[10];
};


#endif //BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
