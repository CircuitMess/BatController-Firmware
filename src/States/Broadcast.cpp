#include "Broadcast.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <Loop/LoopManager.h>
#include <cstdlib>
#include <cstring>
#include <Com/Communication.h>
#include <NetworkConfig.h>


Pair::Broadcast::Broadcast(Pair::PairService *pairService, uint16_t id) : State(pairService), id(id){
}

Pair::Broadcast::~Broadcast(){

}

void Pair::Broadcast::onStart(){
    memcpy(ssid, "Batmobile ", 10);
    ssid[10] = (id / 100) + '0';
    ssid[11] = ((id / 10) % 10) + '0';
    ssid[12] = (id % 10) + '0';
    ssid[13] = '\0';
    Serial.printf("%s\n", ssid);

    memset(password, 0, 10);
    char batmobile[] = "Batmobile";
    for(int i = 0; i < 9; i++){
        char temp =  batmobile[i];
        temp = temp + id * 5 + 16;
        temp = temp % ('z' - 'A') + 'A';
        password[i] = temp;
    }
    password[9] = '\0';

	Serial.print("Setting AP (Access Point)...");
	WiFi.softAP(ssid, password);
	WiFi.softAPConfig(controllerIP, gateway, subnet);

	IPAddress IP = WiFi.softAPIP();
	Serial.print("\nAP IP address: ");
	Serial.printf(IP.toString().c_str());

    Com.begin();

	LoopManager::addListener(this);
}

void Pair::Broadcast::onStop(){
	LoopManager::removeListener(this);
}

void Pair::Broadcast::loop(uint micros){
}

