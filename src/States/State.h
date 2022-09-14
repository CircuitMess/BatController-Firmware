
#ifndef BATCONTROLLER_FIRMWARE_STATE_H
#define BATCONTROLLER_FIRMWARE_STATE_H

#include "PairService.h"


class Pair::State {
public:
	State(Pair::PairService* pairService);
	virtual ~State() = default;

	virtual void start();
	virtual void stop();

protected:
	virtual void onStart() = 0;
	virtual void onStop() = 0;
	Pair::PairService* pairService;

private:
	bool started = false;
};


#endif //BATCONTROLLER_FIRMWARE_STATE_H
