#include "WiFiService.h"
#include <WiFi.h>
#include <BatController.h>
#include <Loop/LoopManager.h>
#include <Settings.h>

WiFiService::WiFiService(){
	WiFi.disconnect();
}

WiFiService::~WiFiService(){
	stop();
}

void WiFiService::start(const char* ssid, const char* pass){
	this->ssid = ssid;
	this->pass = pass;
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, pass);
	LoopManager::addListener(this);
}

void WiFiService::stop(){
	LoopManager::removeListener(this);
}

void WiFiService::loop(uint micros){
	if(WiFi.status() == WL_CONNECTED){
		if(doneCallback) doneCallback(true);
		stop();
		return;
	}

	retryCounter += micros;
	if(retryCounter >= RetryInterval){
		retryCounter = 0;
		retryCount++;

		if(retryCount >= RetryTries){
			if(doneCallback) doneCallback(false);
			stop();
			return;
		}

		WiFi.disconnect(true, true);
		WiFi.begin(ssid, pass);
		delay(100);
	}

	delay(10);
}

void WiFiService::setDoneCallback(std::function<void(bool)> cb){
	this->doneCallback = cb;
}

IPAddress WiFiService::getIPAddress(){
	if(WiFi.status() != WL_CONNECTED) return nullptr;
	return WiFi.localIP();
}

bool WiFiService::isConnected(){
	return WiFi.status() == WL_CONNECTED;
}

