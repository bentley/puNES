/*
 * mapper225.c
 *
 *  Created on: 06/feb/2011
 *      Author: fhorse
 */

#include "mappers.h"
#include "memmap.h"

WORD prgRom32kMax, prgRom16kMax, chrRom8kMax;

void mapInit_225(void) {
	prgRom32kMax = (info.prgRom16kCount >> 1) - 1;
	prgRom16kMax = info.prgRom16kCount - 1;
	chrRom8kMax = info.chrRom8kCount - 1;

	EXTCL_CPU_WR_MEM(225);

	if (info.reset >= HARD) {
		mapPrgRom8k(4, 0, 0);
	}
}
void extcl_cpu_wr_mem_225(WORD address, BYTE value) {
	DBWORD bank;

    value = (address >> 7) & 0x1F;

	if (prgRom32kMax > 0x1F) {
		value |= ((address >> 9) & 0x20);
	}

    if (address & 0x1000) {
    	value = (value << 1) | ((address >> 6) & 0x01);
    	controlBank(prgRom16kMax)
    	mapPrgRom8k(2, 0, value);
    	mapPrgRom8k(2, 2, value);
    } else {
		controlBank(prgRom32kMax)
		mapPrgRom8k(4, 0, value);
    }
	mapPrgRom8kUpdate();

	if (address & 0x2000) {
		mirroring_H();
		/*
		 * workaround per far funzionare correttamente il 52esimo gioco
		 * della rom "52 Games [p1].nes" che non utilizza ne il mirroring
		 * verticale ne quello orizzontale.
		 */
		if ((info.id == BMC52IN1) && (address == 0xA394)) {
			mirroring_FSCR();
		}
	} else {
		mirroring_V();
	}

	value = address & 0x3F;

	if (chrRom8kMax > 0x3F) {
		value |= ((address >> 8) & 0x40);
	}

	controlBank(chrRom8kMax)
	bank = value << 13;
	chr.bank1k[0] = &chr.data[bank];
	chr.bank1k[1] = &chr.data[bank | 0x0400];
	chr.bank1k[2] = &chr.data[bank | 0x0800];
	chr.bank1k[3] = &chr.data[bank | 0x0C00];
	chr.bank1k[4] = &chr.data[bank | 0x1000];
	chr.bank1k[5] = &chr.data[bank | 0x1400];
	chr.bank1k[6] = &chr.data[bank | 0x1800];
	chr.bank1k[7] = &chr.data[bank | 0x1C00];
}
