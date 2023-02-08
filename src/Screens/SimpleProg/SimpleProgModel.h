#ifndef BATCONTROLLER_FIRMWARE_SIMPLEPROGMODEL_H
#define BATCONTROLLER_FIRMWARE_SIMPLEPROGMODEL_H

#include <Arduino.h>
#include <DriveDirection.h>
#include <string>

//TODO - define static color array used in Action's RGBData
namespace Simple {
	enum Color : uint8_t {
		Off		= 0b0000,
		Red		= 0b0001,
		Green	= 0b0010,
		Yellow	= 0b0011,
		Blue	= 0b0100,
		Magenta	= 0b0101,
		Cyan	= 0b0110,
		White	= 0b0111
	};

	struct Action {
		enum class Type : uint8_t {
			Drive, Headlights, Taillights, Underlights, Sound, Delay
		} type;

		union {
			uint8_t data[3];

			struct {
				DriveDirection dir;
				uint8_t duration; //duration increment is 100ms
				uint8_t speed;
			} DriveData;

			struct {
				bool toggle;
			} HeadTaillightData;

			struct {
				Color color;
			} RGBData;

			struct {
				uint8_t sampleIndex;
			} SoundData;

			struct {
				uint8_t duration;
			} DelayData;
		};
	};

	struct Program {
		std::vector<Action> actions;
		std::string name;
	};
}

extern const char* DirectionSymbols[4];
extern const uint8_t SoundsNum;
extern const char* SoundNames[];
extern const uint8_t ColorsNum;
extern const char* ColorNames[];


#endif //BATCONTROLLER_FIRMWARE_SIMPLEPROGMODEL_H
