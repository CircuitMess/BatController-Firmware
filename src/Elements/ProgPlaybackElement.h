#ifndef BATCONTROLLER_FIRMWARE_PROGPLAYBACKELEMENT_H
#define BATCONTROLLER_FIRMWARE_PROGPLAYBACKELEMENT_H

#include "../Screens/SimpleProg/SimpleProgModel.h"
#include <LVObject.h>

class ProgPlaybackElement : public LVObject {
public:
	explicit ProgPlaybackElement(lv_obj_t* parent, const Simple::Program& program);
	~ProgPlaybackElement() override;
	void nextAction();

private:
	lv_obj_t* doneElement;
	lv_obj_t* doneLabel;
	lv_style_t border;
	lv_style_t noBorder;
	const Simple::Program program;

	size_t actionCursor = 0;
	bool scrolling = false;
};


#endif //BATCONTROLLER_FIRMWARE_PROGPLAYBACKELEMENT_H
