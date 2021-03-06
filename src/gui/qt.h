/*
 *  Copyright (C) 2010-2016 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef QT_H_
#define QT_H_

#if defined (__WIN32__)
#include "win.h"
#else
#include <sys/time.h>
#endif
#include "common.h"
#include "emu.h"
#include "jstick.h"

#if defined (__cplusplus)
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC struct _gui {
#if defined (__WIN32__)
	char home[MAX_PATH];
	DWORD version_os;
	double frequency;
	uint64_t counter_start;
#else
	const char *home;
	struct timeval counterStart;
#endif

	char last_open_path[LENGTH_FILE_NAME_MAX];

	//int8_t cpu_cores;

	uint8_t start;
	uint8_t in_update;

	/* lost focus pause */
	uint8_t main_win_lfp;

	int dlg_rc;

	struct _key {
		DBWORD tl;
	} key;
} gui;
EXTERNC struct _mouse {
	int x;
	int y;
	uint8_t left;
	uint8_t right;

	uint8_t hidden;

	double timer;
} mouse;

EXTERNC void gui_quit(void);
EXTERNC BYTE gui_create(void);
EXTERNC void gui_start(void);

EXTERNC void gui_set_video_mode(void);

EXTERNC void gui_update(void);

EXTERNC void gui_fullscreen(void);
EXTERNC void gui_timeline(void);
EXTERNC void gui_save_slot(BYTE slot);

EXTERNC void gui_flush(void);
EXTERNC void gui_print_usage(char *usage);
EXTERNC void gui_reset_video(void);
EXTERNC int gui_uncompress_selection_dialog();

EXTERNC void gui_after_set_video_mode(void);
EXTERNC void gui_set_focus(void);

EXTERNC void gui_cheat_init(void);
EXTERNC void gui_cheat_read_game_cheats(void);
EXTERNC void gui_cheat_save_game_cheats(void);

EXTERNC void gui_cursor_init(void);
EXTERNC void gui_cursor_set(void);
EXTERNC void gui_cursor_hide(BYTE hide);
EXTERNC void gui_control_visible_cursor(void);

EXTERNC void gui_mainWindow_make_reset(BYTE type);

EXTERNC double (*gui_get_ms)(void);

EXTERNC void gui_init(int *argc, char **argv);
EXTERNC void gui_sleep(double ms);
#if defined (__WIN32__)
EXTERNC HWND gui_screen_id(void);
#else
EXTERNC int gui_screen_id(void);
#endif

//EXTERNC void gui_add_event(void *funct, void *args);
//EXTERNC void gui_set_thread_affinity(uint8_t core);

#undef EXTERNC

#endif /* QT_H_ */
