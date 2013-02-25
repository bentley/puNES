/*
 * ppuinline.h
 *
 *  Created on: 23/apr/2010
 *      Author: fhorse
 */

#ifndef PPUINLINE_H_
#define PPUINLINE_H_

#include "external_calls.h"

static BYTE INLINE ppuRdMem(WORD address);

static BYTE INLINE ppuRdMem(WORD address) {
	address &= 0x3FFF;
	if (address < 0x2000) {
		if (extcl_rd_chr) {
			/*
			 * utilizzato dalle mappers :
			 * MMC5
			 */
			return (extcl_rd_chr(address));
		}
		return (chr.bank1k[address >> 10][address & 0x3FF]);
	}
	if (address < 0x3F00) {
		address &= 0x0FFF;
		if (extcl_rd_nmt) {
			/*
			 * utilizzato dalle mappers :
			 * MMC5
			 * Bandai (B161X02X74)
			 */
			return (extcl_rd_nmt(address));
		}
		return (ntbl.bank1k[address >> 10][address & 0x3FF]);
	}
	return (palette.color[address & 0x1F]);
}

#endif /* PPUINLINE_H_ */
