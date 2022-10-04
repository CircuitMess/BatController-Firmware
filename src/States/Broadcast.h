
#ifndef BATCONTROLLER_FIRMWARE_BROADCAST_H
#define BATCONTROLLER_FIRMWARE_BROADCAST_H
#include "State.h"
#include <Loop/LoopListener.h>
#include <WiFiClient.h>

class Pair::Broadcast : public State, private LoopListener{
public:
	Broadcast(Pair::PairService *pairService, uint16_t id);
	~Broadcast();
	void loop(uint micros) override;
protected:
	void onStart() override;
	void onStop() override;
private:
    uint16_t id;
    const char* ssid;
    char password[10];
};


#endif //BATCONTROLLER_FIRMWARE_BROADCAST_H
