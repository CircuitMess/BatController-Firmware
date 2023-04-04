#ifndef BATCONTROLLER_FIRMWARE_PROMPT_H
#define BATCONTROLLER_FIRMWARE_PROMPT_H

#include <LVModal.h>

#define EV_PROMPT_YES ((lv_event_code_t) (_LV_EVENT_LAST + 1))
#define EV_PROMPT_NO ((lv_event_code_t) (_LV_EVENT_LAST + 2))

class Prompt : public LVModal {
public:
	Prompt(LVScreen* parent, const char* text);
	~Prompt() override;

protected:
	void onStart() override;
	void onStop() override;

private:
	lv_style_t btnDefault;
	lv_style_t btnFocused;

	lv_obj_t* noBtn;
	lv_obj_t* yesBtn;
};


#endif //BATCONTROLLER_FIRMWARE_PROMPT_H
