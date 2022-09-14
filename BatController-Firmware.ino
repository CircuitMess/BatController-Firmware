#include <Arduino.h>
#include <CircuitOS.h>
#include <BatController.h>

void setup(){
	BatController.begin();

	BatController.getDisplay()->clear(TFT_RED);
	BatController.getDisplay()->commit();
}

void loop(){

}