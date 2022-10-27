#include "Driver.h"

Driver::Driver(Feed& feed, lv_obj_t* elementContainer) : feed(feed), elementContainer(elementContainer){}

void Driver::start(){
	onStart();
}

void Driver::stop(){
	onStop();
}
