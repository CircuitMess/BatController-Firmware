#define ICACHE_RODATA_ATTR  __attribute__((section(".irom.text")))
#define PROGMEM   ICACHE_RODATA_ATTR

#include "Broadcast.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <Loop/LoopManager.h>
#include <cstdlib>
#include "../NetworkConfig.h"
#include <cstring>

AsyncServer* server;



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

    server = new AsyncServer(port);

    server->onClient([this](void* arg, AsyncClient* client){

        Serial.println("new client connected");

        client->onConnect([this](void*, AsyncClient* server){
            Serial.println("client connected to port");
        }, nullptr);

        client->onDisconnect([this](void*, AsyncClient* server){
            //TODO - reconnect to wifi in case of total disconnect
            Serial.println("client disconnected from port");
        });

        client->onError([this](void*, AsyncClient* c, int8_t error){
            Serial.print("error occured: ");
            Serial.println(c->errorToString(error));
        }, nullptr);

        client->onData([this](void* arg, AsyncClient* server, void* data, size_t len){
            Serial.printf("CLIENT DATA\n");
            Serial.printf("%s", data);
//            Serial.write((uint8_t *)data, len);
        });

        client->onTimeout([this](void*, AsyncClient*, uint32_t time){
            Serial.printf("timeout error %d passed\n", time);
        }, nullptr);

    }, nullptr);


	server->begin();

	LoopManager::addListener(this);
}

void Pair::Broadcast::onStop(){
	LoopManager::removeListener(this);
}

void Pair::Broadcast::loop(uint micros){
}

