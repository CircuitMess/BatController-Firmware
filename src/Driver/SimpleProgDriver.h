#ifndef BATCONTROLLER_FIRMWARE_SIMPLEPROGDRIVER_H
#define BATCONTROLLER_FIRMWARE_SIMPLEPROGDRIVER_H


#include <Loop/LoopListener.h>
#include "Driver.h"
#include "../Screens/SimpleProg/SimpleProgModel.h"

class SimpleProgDriver : public Driver, private LoopListener {
public:
	SimpleProgDriver(std::unique_ptr<Simple::Program> program);

	~SimpleProgDriver() override{};
	void setContainer(lv_obj_t* container);

protected:
	void onStart() override;
	void onStop() override;

private:
	void loop(uint micros) override;
	void exit();
	std::unique_ptr<Simple::Program> program;

	bool inited = false;
	size_t actionCursor = 0;
	uint32_t actionTimer = 0;
	bool actionExecuted = false;
};


#endif //BATCONTROLLER_FIRMWARE_SIMPLEPROGDRIVER_H
