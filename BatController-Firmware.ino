#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <WiFiClient.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <pgmspace.h>
#include <Loop/LoopManager.h>

Display* display;
Sprite* baseSprite;

void setup(){
	Serial.begin(115200);
	srand(analogRead(A0) + 1 + analogRead(A0) * 2);
	Nibble.begin();
	Nibble.setBacklight(true);

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	Serial.printf("created: %d\n", baseSprite->created());


	int pixelPerTile = 15;
	baseSprite->push();


}

void loop(){
	LoopManager::loop();
	display->commit();
}
