#define ICACHE_RODATA_ATTR  __attribute__((section(".irom.text")))
#define PROGMEM   ICACHE_RODATA_ATTR

#include "Broadcast.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <Loop/LoopManager.h>
#include <cstdlib>
#include "../NetworkConfig.h"

AsyncServer* server;



Pair::Broadcast::Broadcast(Pair::PairService *pairService, uint16_t id) : State(pairService), id(id){
}

Pair::Broadcast::~Broadcast(){

}

void Pair::Broadcast::onStart(){
	char numChar[3];
	std::sprintf(numChar, "%d", id);
	strcat(ssid, numChar);

    char batmobile[8];
    strncpy(batmobile,"Batmobil", sizeof(batmobile));
    for(int i = 0; i < 8; i++){
        char temp =  batmobile[i];
        temp = temp + id * 5 + 16;
        temp = temp % ('z' - 'A') + 'A';
        password[i] = temp;
    }


	Serial.print("Setting AP (Access Point)…");
	WiFi.softAP(ssid, password);
	WiFi.softAPConfig(controllerIP, gateway, subnet);

	IPAddress IP = WiFi.softAPIP();
	Serial.print("\nAP IP address: ");
	Serial.printf(IP.toString().c_str());
    Serial.printf("\nPass: %s", password);
	server.begin();

	LoopManager::addListener(this);
}

void Pair::Broadcast::onStop(){
	LoopManager::removeListener(this);
}

void Pair::Broadcast::loop(uint micros){
}

