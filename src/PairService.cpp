#include "PairService.h"
#include <Arduino.h>
#include <WiFi.h>
#include <cstdlib>
#include <cstring>
#include <Com/Communication.h>
#include <NetworkConfig.h>
#include <Loop/LoopManager.h>

PairService::PairService(){}

PairService::~PairService(){
	stop();
}

void PairService::start(const char* ssid, const char* pass, bool directConnection){
	this->directConnection = directConnection;

	if(server) return;

	Com.setMode(directConnection ? ComMode::Direct : ComMode::External);

	server = Com.getServer();
	if(!server || !server->status()){
		if(doneCallback) doneCallback(PairError::ServerError);
		stop();
		return;
	}

	server->onClient([this](void* arg, AsyncClient* client){
		this->client = std::unique_ptr<AsyncClient>(client);
		this->server->onClient(nullptr, nullptr);
	}, nullptr);

	if(directConnection){
		controllerIP = defaultControllerIP;

		WiFi.mode(WIFI_AP);
		WiFi.softAP(ssid, pass, 1);
		delay(100);
		WiFi.softAPConfig(controllerIP, gateway, subnet);
		LoopManager::addListener(this);

	}else if(!directConnection){
		wifi.start(ssid, pass);
		wifi.setDoneCallback([this](bool wifiConnected){
			if(wifiConnected){
				controllerIP = WiFi.localIP();
				LoopManager::addListener(this);
				if(doneCallback) doneCallback(PairError::ExternalWiFiConnected);
			}else{
				if(doneCallback) doneCallback(PairError::ExternalWiFiTimeout);
				stop();
			}
		});
	}
}

void PairService::stop(){
	wifi.stop();
	LoopManager::removeListener(this);

	if(!server) return;
	server->onClient(nullptr, nullptr);

	server = nullptr;
}

void PairService::loop(uint micros){
	if(!directConnection && !WiFiService::isConnected()){
		if(doneCallback) doneCallback(PairError::ExternalWiFiTimeout);
		stop();
	}
	if(!client) return;

	stop();
	Com.setClient(std::move(client));

	if(doneCallback){
		doneCallback(PairError::PairOk);
	}
}

void PairService::setDoneCallback(std::function<void(PairError)> doneCallback){
	PairService::doneCallback = std::move(doneCallback);
}
