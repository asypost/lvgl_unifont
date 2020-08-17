#ifndef __LV_FONT_UNIFONT_H_
#define __LV_FONT_UNIFONT_H_
#include "lvgl.h"
#if !LV_USE_FILESYSTEM
#error File System is required
#endif
#ifdef __cplusplus
extern "C" {
#endif

#define FONT_FILE "S:/unifont-13.0.02.hex"

extern lv_font_t lv_font_unifont_16;

void lv_font_unifont_16_close();

#ifdef __cplusplus
}
#endif
#endif
