#include "Driver.h"

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
