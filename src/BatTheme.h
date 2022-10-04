#ifndef CHATTER_FIRMWARE_CHATTERTHEME_H
#define CHATTER_FIRMWARE_CHATTERTHEME_H

#include <core/lv_theme.h>
#include "core/lv_obj.h"

void BatThemeInit(lv_disp_t* disp);

lv_theme_t* BatThemeGet(void);

bool BatThemeInited(void);

#endif //CHATTER_FIRMWARE_CHATTERTHEME_H

