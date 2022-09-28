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

    std::string batmobile = "Batmobile";
    int i = 0;
	for(char& c: batmobile){
		int temp = (int) c;
		temp = temp + id * 5 + 16;
		temp = temp % (121 - 64) + 64;
		password[i] = (char) temp;
		i++;
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

