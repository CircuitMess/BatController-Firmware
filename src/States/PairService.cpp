
#include "PairService.h"
#include "State.h"
#include "Broadcast.h"

Pair::PairService::PairService(uint16_t id) {
	currentState = new Pair::Broadcast(this, id);
	currentState->start();
}

Pair::PairService::~PairService(){

}

void Pair::PairService::setState(Pair::State* state){
	currentState->stop();
	delete currentState;
	currentState = state;
	currentState->start();
}
