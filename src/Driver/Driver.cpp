#include "Driver.h"

Driver::Driver(DriveMode mode) : mode(mode){}

void Driver::start(){
	onStart();
}

void Driver::stop(){
	onStop();
}

void Driver::onFrame(const DriveInfo& frame, Color* pixels){

}

Driver::~Driver() {

}

DriveMode Driver::getMode() const{
	return mode;
}
