/*
 * gfx.h
 *
 *  Created on: 01/mar/2013
 *      Author: fhorse
 */

/* definizione funzione virtuale */
#ifndef gfx_filter_function
#define gfx_filter_function(name) void name(WORD *screen, WORD **screen_index, uint32_t *palette,\
		BYTE bpp, uint32_t pitch, void *pix, WORD rows, WORD lines, WORD width, WORD height,\
		BYTE factor)
#endif

#ifndef GFX_H_
#define GFX_H_

#include "common.h"
#include "filters/video/scale.h"
#include "filters/video/scale2x.h"
#include "filters/video/ntsc.h"
#include "filters/video/hqx.h"

enum render_type { RENDER_SOFTWARE, RENDER_HLSL };
enum fullscreen_mode { NO_FULLSCR, FULLSCR };
enum scale_type { X1 = 1, X2, X3, X4 };
enum filters_type {
	NO_FILTER,
	SCALE2X,
	SCALE3X,
	SCALE4X,
	HQ2X,
	HQ3X,
	HQ4X,
	NTSC_FILTER,
	BILINEAR,
	POSPHOR,
	SCANLINE,
	DBL,
	CRT_CURVE,
	CRT_NO_CURVE,
};
enum overcan_type { OSCAN_OFF, OSCAN_ON, OSCAN_DEFAULT, OSCAN_DEFAULT_OFF, OSCAN_DEFAULT_ON };
enum gfx_info_index { CURRENT, NO_OVERSCAN, MONITOR, VIDEO_MODE };
enum no_change { NO_CHANGE = 255 };

struct _gfx {
	BYTE scale_before_fscreen;
	BYTE bit_per_pixel;

	WORD rows;
	WORD lines;

	SDBWORD w[4];
	SDBWORD h[4];

	float w_pr;
	float h_pr;

	gfx_filter_function((*filter));

	struct {
		BYTE compliant;
		BYTE enabled;
		BYTE used;
	} hlsl;
} gfx;

BYTE gfx_init(void);
void gfx_set_render(BYTE render);
void gfx_set_screen(BYTE scale, BYTE filter, BYTE fullscreen, BYTE palette, BYTE force_scale);
void gfx_draw_screen(BYTE forced);
void gfx_reset_video(void);
void gfx_quit(void);

#endif /* GFX_H_ */
