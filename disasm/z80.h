/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2018 by Michael Kohn
 *
 */

#ifndef _DISASM_Z80_H
#define _DISASM_Z80_H

#include "common/assembler.h"

int get_cycle_count_z80(unsigned short int opcode);
int disasm_z80(struct _memory *memory, uint32_t address, char *instruction, int *cycles_min, int *cycles_max);
void list_output_z80(struct _asm_context *asm_context, uint32_t start, uint32_t end);
void disasm_range_z80(struct _memory *memory, uint32_t flags, uint32_t start, uint32_t end);

#endif

