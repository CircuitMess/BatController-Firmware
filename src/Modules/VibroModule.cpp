#include "VibroModule.h"

VibroModule::VibroModule() : Module(0x58){}

void VibroModule::init(){
	if(!aw9523.begin()) errorOccured();
	for(int i = 0; i < 16; ++i){
		if(i == VibroPin){
			aw9523.pinMode(VibroPin, AW9523::OUT);
			setVibro();
		}else{
			aw9523.pinMode(i, AW9523::LED);
			aw9523.dim(i, 255);
		}
	}
	push = false;
}

void VibroModule::transmission(uint micros){
	if(push){
		setVibro();
		setLEDs();
		push = false;
	}
}

void VibroModule::setVibrating(bool on){
	vibro = on;
	push = true;
}

void VibroModule::setLEDFill(uint8_t percentage){
	fillPercentage = percentage;
	push = true;
}

void VibroModule::setVibro(){
	aw9523.write(VibroPin, !vibro);
}

void VibroModule::setLEDs(){
	uint32_t fill = map(fillPercentage, 0, 100, 0, NumLEDs * 255);
	for(int i = 0; i < NumLEDs; ++i){
		uint8_t pixelValue = min(fill, 255U);
		aw9523.dim(mapPin(i), pixelValue);
		if(fill >= 255){
			fill -= 255;
		}else if(fill > 0){
			fill = 0;
		}
	}
}

uint8_t VibroModule::mapPin(uint8_t led){
	if(led <= 3) return 15 - led;
	else if(led <= 11) return 11 - led;
	else return 23 - led;
}
//pins bottom to top: 15, 14, 13, 12, 7, 6, 5, 4, 3, 2, 1, 0, 11, 10, 9

