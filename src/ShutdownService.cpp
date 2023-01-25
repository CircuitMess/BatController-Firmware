#include <Com/Communication.h>
#include <BatController.h>
#include <WiFi.h>
#include <Loop/LoopManager.h>
#include "ShutdownService.h"

ShutdownService AutoShutdown;

void ShutdownService::begin(){
	LoopManager::addListener(this);
}

void ShutdownService::pause(){
	paused = true;
}

void ShutdownService::resume(){
	paused = false;
	timer = 0;
}

void ShutdownService::buttonPressed(uint i){
	timer = 0;
}

void ShutdownService::buttonReleased(uint i){
	timer = 0;
}

void ShutdownService::loop(uint micros){
	if(paused || Settings.get().shutdownTime == 0) return;
	timer += micros;

	if(timer >= ShutdownSeconds[Settings.get().shutdownTime] * 1000000){

		if(Com.isConnected()){
			LoopManager::removeListener(this);
			Com.sendShutdown([this](bool ackReceived){
				shutdown();
			});
		}else{
			LoopManager::removeListener(this);
			shutdown();
		}
	}
}

void ShutdownService::shutdown(){
	if(BatController.backlightPowered()){
		BatController.fadeOut();
	}

	adc_power_off();
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);

	ledcDetachPin(PIN_BL);

	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
	esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
	esp_deep_sleep_start();
}
