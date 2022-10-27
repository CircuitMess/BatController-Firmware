#ifndef BATCONTROLLER_FIRMWARE_BOOSTELEMENT_H
#define BATCONTROLLER_FIRMWARE_BOOSTELEMENT_H

#include "../Interface/LVObject.h"

class BoostElement : public LVObject {
public:

	explicit BoostElement(lv_obj_t* parent);
/**
	 * @param active True - on (fire), False - off (no fire)
	 */
	void setActive(bool active);
	/**
	 * @param level [0-100]
	 */
	void setLevel(uint8_t level);

private:
	lv_obj_t* bar;
	lv_obj_t* bgGIF;

	bool gifRunning = false; //to prevent gif restarting on subsequent setActive(true) calls

	static constexpr uint8_t w = 27;
	static constexpr uint8_t h = 118;

	const char* fullPath = "S:/DriveScreen/Boost_full.bin";
	const char* emptyPath = "S:/DriveScreen/Boost_empty.bin";
	const char* activePath = "S:/DriveScreen/Boost_fire.gif";
	const char* fillSegmentPath = "S:/DriveScreen/Boost_segment.bin";
};


#endif //BATCONTROLLER_FIRMWARE_BOOSTELEMENT_H
