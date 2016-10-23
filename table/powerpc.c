/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2010-2016 by Michael Kohn
 *
 */

#include "table/powerpc.h"

struct _table_powerpc table_powerpc[] =
{
  // Aliases
  { "blr",    0x4e800020, 0xffffffff, OP_NONE, FLAG_NONE, 0, 0 },
  { "li",     0x38000000, 0xfc0f0000, OP_RD_SIMM, FLAG_NONE, 0, 0 },

  // Instructions
  { "add",    0x7c000214, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "addo",   0x7c000614, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "addc",   0x7c000014, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "addco",  0x7c000414, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "adde",   0x7c000114, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "addeo",  0x7c000514, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "addi",   0x38000000, 0xfc000000, OP_RD_RA_SIMM, FLAG_NONE, 0, 0 },
  { "addic",  0x30000000, 0xfc000000, OP_RD_RA_SIMM, FLAG_NONE, 0, 0 },
  { "addic",  0x34000000, 0xfc000000, OP_RD_RA_SIMM, FLAG_REQUIRE_DOT, 0, 0 },
  { "addis",  0x3c000000, 0xfc000000, OP_RD_RA_SIMM, FLAG_NONE, 0, 0 },
  { "addme",  0x7c0001d4, 0xfc0007fe, OP_RD_RA, FLAG_DOT, 0, 0 },
  { "addmeo", 0x7c0005d4, 0xfc00fffe, OP_RD_RA, FLAG_DOT, 0, 0 },
  { "addze",  0x7c000194, 0xfc0007fe, OP_RD_RA, FLAG_DOT, 0, 0 },
  { "addzeo", 0x7c000594, 0xfc00fffe, OP_RD_RA, FLAG_DOT, 0, 0 },
  { "and",    0x7c000038, 0xfc0007fe, OP_RA_RS_RB, FLAG_DOT, 0, 0 },
  { "andis",  0x74000000, 0xfc000000, OP_RA_RS_UIMM, FLAG_REQUIRE_DOT, 0, 0 },
  { "b",      0x48000000, 0xfc000003, OP_BRANCH, FLAG_NONE, 0, 0 },
  { "ba",     0x48000002, 0xfc000003, OP_JUMP, FLAG_NONE, 0, 0 },
  { "bl",     0x48000001, 0xfc000003, OP_BRANCH, FLAG_NONE, 0, 0 },
  { "bla",    0x48000003, 0xfc000003, OP_JUMP, FLAG_NONE, 0, 0 },
  { "bc",     0x40000000, 0xfc000003, OP_BRANCH_COND_BD, FLAG_NONE, 0, 0 },
  { "bca",    0x40000002, 0xfc000003, OP_JUMP_COND_BD, FLAG_NONE, 0, 0 },
  { "bcl",    0x40000001, 0xfc000003, OP_BRANCH_COND_BD, FLAG_NONE, 0, 0 },
  { "bcla",   0x40000003, 0xfc000003, OP_JUMP_COND_BD, FLAG_NONE, 0, 0 },
  { "bcctr",  0x4c000000, 0xfc00ffff, OP_BRANCH_COND, FLAG_NONE, 0, 0 },
  { "bcctrl", 0x4c000001, 0xfc00ffff, OP_BRANCH_COND, FLAG_NONE, 0, 0 },
  { "bclr",   0x4c000040, 0xfc00ffff, OP_BRANCH_COND, FLAG_NONE, 0, 0 },
  { "bclrl",  0x4c000041, 0xfc00ffff, OP_BRANCH_COND, FLAG_NONE, 0, 0 },
  { "cmpd",   0x7c000000, 0xfc6007ff, OP_CMP, FLAG_NONE, 0, 0 },
  { "cmpw",   0x7c200000, 0xfc6007ff, OP_CMP, FLAG_NONE, 0, 0 },
  { "cmpwi",  0x2c000000, 0xfc600000, OP_CMPI, FLAG_NONE, 0, 0 },
  { "cmpdi",  0x2c200000, 0xfc600000, OP_CMPI, FLAG_NONE, 0, 0 },
  { "cntlzw", 0x7c000034, 0xfc0007fe, OP_RA_RS, FLAG_DOT, 0, 0 },
  { "crand",  0x4c000202, 0xfc0007ff, OP_CRB_CRB_CRB, FLAG_NONE, 0, 0 },
  { "crandb", 0x4c000102, 0xfc0007ff, OP_CRB_CRB_CRB, FLAG_NONE, 0, 0 },
  { "creqv",  0x4c000242, 0xfc0007ff, OP_CRB_CRB_CRB, FLAG_NONE, 0, 0 },
  { "crnand", 0x4c0001fe, 0xfc0007ff, OP_CRB_CRB_CRB, FLAG_NONE, 0, 0 },
  { "cnror",  0x4c000042, 0xfc0007ff, OP_CRB_CRB_CRB, FLAG_NONE, 0, 0 },
  { "cror",   0x4c000382, 0xfc0007ff, OP_CRB_CRB_CRB, FLAG_NONE, 0, 0 },
  { "crorc",  0x4c000342, 0xfc0007ff, OP_CRB_CRB_CRB, FLAG_NONE, 0, 0 },
  { "crxor",  0x4c000182, 0xfc0007ff, OP_CRB_CRB_CRB, FLAG_NONE, 0, 0 },
  { "divw",   0x7c0003d6, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "divwo",  0x7c0007d6, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "divwu",  0x7c000396, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "divwuo", 0x7c000796, 0xfc0007fe, OP_RD_RA_RB, FLAG_DOT, 0, 0 },
  { "eqv",    0x7c000238, 0xfc0007fe, OP_RA_RS_RB, FLAG_DOT, 0, 0 },
  { "extsb",  0x7c000774, 0xfc0007fe, OP_RA_RS, FLAG_DOT, 0, 0 },
  { "extsh",  0x7c000734, 0xfc0007fe, OP_RA_RS, FLAG_DOT, 0, 0 },

// blt, bne, bdnz
  //{ "blcr", 0x4c000000, 0xfc00f801, OP_BRANCH, FLAG_NONE, 0, 0 },
  //{ "blcrl", 0x4c000001, 0xfc00f801, OP_BRANCH, FLAG_NONE, 0, 0 },
  { NULL, 0, 0, 0, 0 }
};

