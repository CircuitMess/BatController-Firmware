#ifndef WHEELSON_FIRMWARE_STORAGE_H
#define WHEELSON_FIRMWARE_STORAGE_H

#include <Arduino.h>
#include <vector>
#include "SimpleProgModel.h"

namespace Simple {

	class Storage {
	public:
		Storage();
		~Storage();

		[[nodiscard]] uint8_t getNumProgs() const;
		Program getProg(uint8_t index);
		void addProg(const Program& program);
		void removeProg(uint8_t index);
		void updateProg(uint8_t index, const Program& program);
		void clearAll();

		bool nameTaken(std::string name);

	private:
		void readProgs();
		void writeProgs();

		std::vector<Program> programs;
		static constexpr const char* filePath = "/SimpleProgs.bin";
	};

}


#endif //WHEELSON_FIRMWARE_STORAGE_H
