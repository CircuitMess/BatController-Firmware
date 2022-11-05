#include "PairService.h"
#include <Arduino.h>
#include <WiFi.h>
#include <cstdlib>
#include <cstring>
#include <Com/Communication.h>
#include <NetworkConfig.h>
#include <Loop/LoopManager.h>

PairService::PairService(){ }

PairService::~PairService(){
	stop();
}

void PairService::start(uint16_t id){
	if(server) return;

	server = Com.getServer();
	if(!server || !server->status()) return;

	server->onClient([this](void* arg, AsyncClient* client){
		this->client = std::unique_ptr<AsyncClient>(client);
		this->server->onClient(nullptr, nullptr);
	}, nullptr);

	memcpy(ssid, "Batmobile ", 10);
	ssid[10] = (id / 100) + '0';
	ssid[11] = ((id / 10) % 10) + '0';
	ssid[12] = (id % 10) + '0';
	ssid[13] = '\0';

	memset(password, 0, 10);
	char batmobile[] = "Batmobile";
	for(int i = 0; i < 9; i++){
		char temp = batmobile[i];
		temp = temp + id * 5 + 16;
		temp = temp % ('z' - 'A') + 'A';
		password[i] = temp;
	}
	password[9] = '\0';

	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password, 1, 1);
	delay(100);
	WiFi.softAPConfig(controllerIP, gateway, subnet);

	LoopManager::addListener(this);
}

void PairService::stop(){
	LoopManager::removeListener(this);

	if(!server) return;
	server->onClient(nullptr, nullptr);

	server = nullptr;
}

void PairService::loop(uint micros){
	if(!client) return;

	stop();
	Com.setClient(std::move(client));

	if(doneCallback){
		doneCallback();
	}
}

void PairService::setDoneCallback(std::function<void()> doneCallback){
	PairService::doneCallback = std::move(doneCallback);
}
