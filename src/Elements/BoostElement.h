#ifndef BATCONTROLLER_FIRMWARE_BOOSTELEMENT_H
#define BATCONTROLLER_FIRMWARE_BOOSTELEMENT_H

#include <Loop/LoopListener.h>
#include <LVObject.h>

class BoostElement : public LVObject, private LoopListener {
public:

	explicit BoostElement(lv_obj_t* parent);
	virtual ~BoostElement();

	/**
	 * @param active True - on (fire), False - off (no fire)
	 */
	void setActive(bool active);

	bool isActive() const;

	/**
	 * @param level [0-100]
	 */
	void setLevel(uint8_t level);

private:
	lv_obj_t* bar;
	lv_obj_t* bgIMG;

	bool gifRunning = false; //to prevent gif restarting on subsequent setActive(true) calls
	uint8_t frame = 0;
	uint32_t frameTime = 0;
	void loop(uint micros) override;
	void nextFrame();

	static constexpr uint8_t w = 27;
	static constexpr uint8_t h = 118;

	static constexpr const char* fullPath = "S:/DriveScreen/Boost_full.bin";
	static constexpr const char* emptyPath = "S:/DriveScreen/Boost_empty.bin";
	static constexpr const char* activePath = "S:/DriveScreen/Boost_fire.gif";
	static constexpr const char* fillSegmentPath = "S:/DriveScreen/Boost_segment.bin";
};


#endif //BATCONTROLLER_FIRMWARE_BOOSTELEMENT_H
