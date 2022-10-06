

#include "Broadcast.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <Loop/LoopManager.h>
#include <cstdlib>
#include <cstring>
#include <Communication.h>
#include <NetworkConfig.h>


Pair::Broadcast::Broadcast(Pair::PairService *pairService, uint16_t id) : State(pairService), id(id){
}

Pair::Broadcast::~Broadcast(){

}

void Pair::Broadcast
::onStart(){
    String ssidString = "Batmobile ";
    ssidString += String(id);
    Serial.printf("%s\n",ssidString.c_str());
    ssid = ssidString.c_str();
//    ssidString.toCharArray(ssid,ssidString.length() + 1);
    memset(password, 0, 10);
    String batmobile = "Batmobile";
    for(int i = 0; i < 9; i++){
        char temp =  batmobile[i];
        temp = temp + id * 5 + 16;
        temp = temp % ('z' - 'A') + 'A';
        password[i] = temp;
    }
    Serial.printf("\nPass: %s\n", password);

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

