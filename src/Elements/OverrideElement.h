#ifndef BATCONTROLLER_FIRMWARE_OVERRIDEELEMENT_H
#define BATCONTROLLER_FIRMWARE_OVERRIDEELEMENT_H

#include <LVObject.h>

enum overrideText {
	Manual, Auto
};

class OverrideElement : public LVObject {
public:
	OverrideElement(lv_obj_t* parent);
	~OverrideElement() override;

	void fill(uint8_t percent);
	void setText(overrideText text);

private:
	lv_obj_t* text;
	lv_obj_t* slider;

	static constexpr size_t w = 79;
	static constexpr size_t h = 48;
	static constexpr uint8_t sliderRange = 31;
};


#endif //BATCONTROLLER_FIRMWARE_OVERRIDEELEMENT_H
