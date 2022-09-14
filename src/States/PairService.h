
#ifndef BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
#define BATCONTROLLER_FIRMWARE_PAIRSERVICE_H



namespace Pair{
	class State;
	class PairService;
	class Broadcast;
}
class Pair::PairService {
public:
	PairService();
	~PairService();
	void setState(Pair::State* state);
private:
	Pair::State* currentState;
};


#endif //BATCONTROLLER_FIRMWARE_PAIRSERVICE_H
