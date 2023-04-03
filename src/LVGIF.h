#ifndef BATCONTROLLER_FIRMWARE_LVGIF_H
#define BATCONTROLLER_FIRMWARE_LVGIF_H

#define LV_USE_GIF 1

#include "Interface/LVObject.h"
#include <vector>
#include <functional>

class LVGIF : public LVObject {
public:
	LVGIF(lv_obj_t* parent, const char* path);
	~LVGIF() override;

	void start();
	void stop();
	void reset();
	void setLooping(lv_gif_loop_t loop);
	void setDoneCallback(std::function<void()> cb);
	void setImage(uint index);
	[[nodiscard]] size_t getNumFrames() const;

private:
	lv_obj_t* img;
	lv_timer_t* timer;

	uint16_t w = 0, h = 0;
	std::vector<uint16_t> durations; //in ms
	uint32_t index = 0;

	const char* path;
	uint32_t pathLen = 0;
	char* imgPath;

	std::function<void()> cb;
};


#endif //BATCONTROLLER_FIRMWARE_LVGIF_H
