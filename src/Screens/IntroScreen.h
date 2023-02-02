#ifndef BATCONTROLLER_FIRMWARE_INTROSCREEN_H
#define BATCONTROLLER_FIRMWARE_INTROSCREEN_H

#include "../Interface/LVScreen.h"
#include <Loop/LoopListener.h>
#include <functional>

class IntroScreen : public LVScreen, private LoopListener {
public:
	IntroScreen();

	void onStart() override;
	void onStop() override;

	/** Called before creating PairScreen */
	void setPreCallback(std::function<void()> preCallback);

	/** Called after creating PairScreen */
	void setPostCallback(std::function<void()> postCallback);

private:
	std::function<void()> preCallback;
	std::function<void()> postCallback;

	void loop(uint micros) override;

	uint32_t currentFrame = 0;
	uint32_t frameTime;

	lv_obj_t* img;

	static constexpr auto FadeTime = 300000;
	uint32_t fade;
	bool blInited = false;

};


#endif //BATCONTROLLER_FIRMWARE_INTROSCREEN_H
