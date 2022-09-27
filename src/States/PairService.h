
#ifndef BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
#define BATCONTROLLER_FIRMWARE_PAIRSERVICE_H


#include <cstdint>

namespace Pair{
	class State;
	class PairService;
	class Broadcast;
}
class Pair::PairService {
public:
	PairService(uint16_t id);
	~PairService();
	void setState(Pair::State* state);
private:
	Pair::State* currentState;
};


#endif //BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
