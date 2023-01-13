#include "SimpleProgScreen.h"

SimpleProgScreen::SimpleProgScreen(){


	//gettanje svih programa
	for(int i = 0; i < storage.getNumProgs(); ++i){
		auto prog = storage.getProg(i);
	}

	//krenemo editirati jednog
	Simple::Program p = storage.getProg(0);

}

void SimpleProgScreen::onStart(){
	LVScreen::onStart();
}

void SimpleProgScreen::onStop(){
	LVScreen::onStop();
}

void SimpleProgScreen::onDisconnected(){
	DisconnectListener::onDisconnected();
}
