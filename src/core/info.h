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

#ifndef INFO_H_
#define INFO_H_

#include "common.h"

#if defined (__cplusplus)
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#define _rom_file char rom_file[LENGTH_FILE_NAME_MID]
typedef struct {
	struct _info_sha1sum_prg {
		BYTE value[20];
		char string[41];
	} prg;
	struct _info_sha1sum_chr {
		BYTE value[20];
		char string[41];
	} chr;
} _info_sh1sum;

EXTERNC struct _info {
	char base_folder[LENGTH_FILE_NAME_MID];
	_rom_file;
	char load_rom_file[LENGTH_FILE_NAME_MID];
	BYTE format;
	BYTE machine[2];
	struct _info_mapper {
		WORD id;
		BYTE submapper;
		BYTE extend_wr;
		BYTE extend_rd;
	} mapper;
	BYTE mirroring_db;
	BYTE portable;
	BYTE id;
	BYTE trainer;
	BYTE stop;
	BYTE reset;
	BYTE execute_cpu;
	BYTE gui;
	BYTE no_rom;
	BYTE uncompress_rom;
	BYTE pause;
	BYTE on_cfg;
	BYTE pause_frames_drawscreen;
	BYTE first_illegal_opcode;
	_info_sh1sum sha1sum;
	struct _info_chr {
		struct _info_chr_rom {
			WORD banks_8k;
			WORD banks_4k;
			WORD banks_1k;
			struct _info_chr_rom_max {
				WORD banks_8k;
				WORD banks_4k;
				WORD banks_2k;
				WORD banks_1k;
			} max;
		} rom;
	} chr;
	struct _info_prg {
		struct _info_prg_rom {
			WORD banks_16k;
			WORD banks_8k;
			struct _info_prg_rom_max {
				WORD banks_32k;
				WORD banks_16k;
				WORD banks_8k;
				WORD banks_8k_before_last;
				WORD banks_4k;
			} max;
		} rom;
		struct _info_prg_ram {
			BYTE banks_8k_plus;
			struct _info_prg_ram_bat {
				BYTE banks;
				BYTE start;
			} bat;
		} ram;
	} prg;
	BYTE r4016_dmc_double_read_disabled;
	BYTE r2002_race_condition_disabled;
	BYTE r4014_precise_timing_disabled;

#if !defined (RELEASE)
	BYTE snd_info;
#endif
} info;

#undef EXTERNC

#endif /* INFO_H_ */
