
#ifndef BATCONTROLLER_FIRMWARE_BROADCAST_H
#define BATCONTROLLER_FIRMWARE_BROADCAST_H
#include "State.h"
#include <Loop/LoopListener.h>
#include <WiFiClient.h>

class Pair::Broadcast : public State, private LoopListener{
public:
	Broadcast(Pair::PairService* pairService);
	~Broadcast();
	void loop(uint micros) override;
protected:
	void onStart() override;
	void onStop() override;
private:
};


#endif //BATCONTROLLER_FIRMWARE_BROADCAST_H
