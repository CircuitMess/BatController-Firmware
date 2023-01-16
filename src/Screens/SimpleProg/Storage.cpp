#include "Storage.h"
#include <SPIFFS.h>
#include <set>

static const char* tag = "SimpleProgStorage";

Simple::Storage::Storage(){
	if(SPIFFS.exists(filePath)){
		readProgs();
	}else{
		writeProgs();
	}
}

Simple::Storage::~Storage(){

}

uint8_t Simple::Storage::getNumProgs() const{
	return programs.size();
}

const Simple::Program& Simple::Storage::getProg(uint8_t index){
	return programs[index];
}

void Simple::Storage::addProg(const Program& program){
	programs.push_back(program);
	writeProgs();
}

void Simple::Storage::removeProg(uint8_t index){
	if(index >= programs.size()) return;

	programs.erase(programs.begin() + index);
	writeProgs();
}

void Simple::Storage::updateProg(uint8_t index, const Program& program){
	if(index >= programs.size()) return;
	programs[index] = program;
	writeProgs();
}

void Simple::Storage::readProgs(){
	File progsFile = SPIFFS.open(filePath, "r");
	if(!progsFile){
		ESP_LOGE(tag, "Error opening SimpleProgs.bin\n");
		return;
	}
	uint8_t numProgs;
	progsFile.read(&numProgs, 1);

	for(uint8_t i = 0; i < numProgs; i++){
		Program program;
		uint8_t numActions;
		progsFile.read(&numActions, 1);

		for(int j = 0; j < numActions; j++){
			Action action;
			uint8_t read = progsFile.read((uint8_t*) &action, sizeof(Action));
			if(read == sizeof(Action)){
				program.actions.push_back(action);
			}else{
				break;
			}
		}

		if(program.actions.size() != numActions) break;

		programs.push_back(program);
	}

	progsFile.close();
}

void Simple::Storage::writeProgs(){
	File progsFile = SPIFFS.open(filePath, "w");
	if(!progsFile){
		ESP_LOGE(tag, "Error opening SimpleProgs.bin\n");
		return;
	}
	uint8_t numProgs = programs.size();
	progsFile.write(numProgs);
	for(auto& prog: programs){
		//write prog to file
		uint8_t numActions = prog.actions.size();
		progsFile.write(numActions);
		for(int j = 0; j < numActions; j++){
			progsFile.write((uint8_t*) &prog.actions[j], sizeof(Action));
		}
	}
	progsFile.close();
}
