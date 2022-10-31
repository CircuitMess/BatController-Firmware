#include "PairService.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <Loop/LoopManager.h>
#include <cstdlib>
#include <cstring>
#include <Com/Communication.h>
#include <NetworkConfig.h>

PairService::PairService(uint16_t id) : id(id) {}

PairService::~PairService(){}

void PairService::broadcast(){
	memcpy(ssid, "Batmobile ", 10);
	ssid[10] = (id / 100) + '0';
	ssid[11] = ((id / 10) % 10) + '0';
	ssid[12] = (id % 10) + '0';
	ssid[13] = '\0';

	memset(password, 0, 10);
	char batmobile[] = "Batmobile";
	for(int i = 0; i < 9; i++){
		char temp =  batmobile[i];
		temp = temp + id * 5 + 16;
		temp = temp % ('z' - 'A') + 'A';
		password[i] = temp;
	}
	password[9] = '\0';

	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password, 1, 1, 1);
	delay(100);
	WiFi.softAPConfig(controllerIP, gateway, subnet);

	Com.begin();
}
