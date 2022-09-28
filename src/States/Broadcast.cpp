#define ICACHE_RODATA_ATTR  __attribute__((section(".irom.text")))
#define PROGMEM   ICACHE_RODATA_ATTR

#include "Broadcast.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Loop/LoopManager.h>
#include <cstdlib>


AsyncWebServer server(63);
char* ssid = "Batmobile ";
char password[9];

IPAddress local_IP(10, 0, 0, 1);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 252);


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
	WiFi.softAPConfig(local_IP, gateway, subnet);

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

