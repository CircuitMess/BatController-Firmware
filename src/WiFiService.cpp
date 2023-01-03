#include "WiFiService.h"
#include <WiFi.h>
#include <BatController.h>
#include <Loop/LoopManager.h>
#include <Settings.h>

WiFiService::WiFiService(){
	WiFi.disconnect();
}

void WiFiService::start(){
	WiFi.mode(WIFI_STA);
	WiFi.begin(Settings.get().ssid, Settings.get().password);
	LoopManager::addListener(this);
}

void WiFiService::stop(){
	LoopManager::removeListener(this);
}

void WiFiService::loop(uint micros){
	if(WiFi.status() != WL_CONNECTED) return;
	stop();
	if(doneCallback){
		doneCallback();
	}
}

void WiFiService::setDoneCallback(std::function<void()> cb){
	this->doneCallback = cb;
}

IPAddress WiFiService::getIPAddress(){
	if(WiFi.status() != WL_CONNECTED) return nullptr;
	return WiFi.localIP();
}