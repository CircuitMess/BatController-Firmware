#ifndef BATCONTROLLER_FIRMWARE_SIMPLEPROGDRIVER_H
#define BATCONTROLLER_FIRMWARE_SIMPLEPROGDRIVER_H

#include <Loop/LoopListener.h>
#include "Driver.h"
#include "../Screens/SimpleProg/SimpleProgModel.h"
#include "../Elements/ProgPlaybackElement.h"

class SimpleProgDriver : public Driver, private LoopListener {
public:
	SimpleProgDriver(std::unique_ptr<Simple::Program> program);

	~SimpleProgDriver() override = default;;
	void setContainer(lv_obj_t* container);

protected:
	void onStart() override;
	void onStop() override;

private:
	void loop(uint micros) override;
	void nextAction();

	std::unique_ptr<Simple::Program> program;
	std::unique_ptr<ProgPlaybackElement> playbackElement;

	bool inited = false;
	size_t actionCursor = 0;
	uint32_t actionTimer = 0;
	bool actionExecuted = false;
};


#endif //BATCONTROLLER_FIRMWARE_SIMPLEPROGDRIVER_H
