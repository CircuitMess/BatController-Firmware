#include "SimpleProgDriver.h"
#include "../Screens/SimpleProg/SimpleProgScreen.h"
#include <Loop/LoopManager.h>
#include <Com/Communication.h>

SimpleProgDriver::SimpleProgDriver(std::unique_ptr<Simple::Program> program) : program(std::move(program)){

}

void SimpleProgDriver::setContainer(lv_obj_t* container){
	inited = true;
	//TODO - build UI once we have a container
}

void SimpleProgDriver::onStart(){
	if(!inited) return;

	actionTimer = 0;
	actionCursor = 0;

	LoopManager::addListener(this);
}

void SimpleProgDriver::onStop(){
	LoopManager::removeListener(this);
}

void SimpleProgDriver::loop(uint micros){

	if(actionCursor >= program->actions.size()){
		stop();
		return;
	}

	auto currentAction = program->actions[actionCursor];

	//timekeeping for actions with durations (Drive and Delay)
	if(currentAction.type == Simple::Action::Type::Drive && actionExecuted){
		actionTimer += micros;
		if(actionTimer >= currentAction.DriveData.duration * 100000){
			actionCursor++;
			actionTimer = 0;
			actionExecuted = false;
			Com.sendDriveDir((uint8_t) DriveDirection::None);
			return;
		}
	}else if(currentAction.type == Simple::Action::Type::Delay && actionExecuted){
		actionTimer += micros;
		if(actionTimer >= currentAction.DelayData.duration * 100000){
			actionCursor++;
			actionTimer = 0;
			actionExecuted = false;
			return;
		}
	}


	if(!actionExecuted){
		switch(currentAction.type){
			case Simple::Action::Type::Drive:
				Com.sendDriveSpeed(constrain(currentAction.DriveData.speed, 0, 100));
				Com.sendDriveDir((uint8_t) currentAction.DriveData.dir);
				actionExecuted = true;
				actionTimer = 0;
				break;

			case Simple::Action::Type::Headlights:
				Com.sendHeadlights(currentAction.HeadTaillightData.toggle ? 255 : 0);
				actionCursor++;
				break;

			case Simple::Action::Type::Taillights:
				Com.sendTaillights(currentAction.HeadTaillightData.toggle ? 255 : 0);
				actionCursor++;
				break;

			case Simple::Action::Type::Underlights:
				Com.sendUnderlights(currentAction.RGBData.colorID);
				actionCursor++;
				break;

			case Simple::Action::Type::Sound:
				Com.sendSoundEffect(currentAction.SoundData.sampleIndex);
				break;

			case Simple::Action::Type::Delay:
				actionExecuted = true;
				actionTimer = 0;
				break;
		}
	}
}
