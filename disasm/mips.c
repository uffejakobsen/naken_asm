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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disasm/mips.h"

#define READ_RAM(a) memory_read_m(memory, a)

int get_cycle_count_mips(uint32_t opcode)
{
  return -1;
}

int disasm_mips(struct _memory *memory, uint32_t address, char *instruction, int *cycles_min, int *cycles_max)
{
  uint32_t opcode;
  int function, format, operation;
  int n, r;
  char temp[32];
  const char *reg[32] =
  {
    "$0", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
  };
  int rs, rt, rd, sa;
  int immediate;

  *cycles_min = 1;
  *cycles_max = 1;
  opcode = memory_read32_m(memory, address);

  instruction[0] = 0;

  if (opcode == 0)
  {
    strcpy(instruction, "nop");
    return 4;
  }

  format = (opcode >> 26) & 0x3f;

  if (format == FORMAT_SPECIAL0 ||
      format == FORMAT_SPECIAL2 ||
      format == FORMAT_SPECIAL3)
  {
    // Special2 / Special3
    function = opcode & 0x3f;

    n = 0;
    while(mips_special_table[n].instr != NULL)
    {
      if (mips_special_table[n].format == format &&
          mips_special_table[n].function == function)
      {
        uint8_t operand_reg[4] = { 0 };
        int shift;

        if (mips_special_table[n].type == SPECIAL_TYPE_REGS)
        {
          operation = (opcode >> 6) & 0x1f;
          shift = 21;
        }
          else
        if (mips_special_table[n].type == SPECIAL_TYPE_SA)
        {
          operation = (opcode >> 21) & 0x1f;
          shift = 16;
        }
          else
        if (mips_special_table[n].type == SPECIAL_TYPE_BITS ||
            mips_special_table[n].type == SPECIAL_TYPE_BITS2)
        {
          operation = 0;
          shift = 21;
        }
          else
        {
          sprintf(instruction, "internal error");
          return 4;
        }

        if (mips_special_table[n].operation != operation)
        {
          n++;
          continue;
        }

        for (r = 0; r < 4; r++)
        {
          int operand_index = mips_special_table[n].operand[r];

          if (operand_index != -1)
          {
            operand_reg[operand_index] = (opcode >> shift) & 0x1f;
          }

          if (r == 2 && mips_special_table[n].type == SPECIAL_TYPE_BITS)
          {
            operand_reg[operand_index]++;
          }
            else
          if (r == 3 && mips_special_table[n].type == SPECIAL_TYPE_BITS2)
          {
            operand_reg[operand_index + 1] -= operand_reg[operand_index];
            operand_reg[operand_index + 1]++;
          }

          shift -= 5;
        }

        strcpy(instruction, mips_special_table[n].instr);

        for (r = 0; r < mips_special_table[n].operand_count; r++)
        {
          if (r < 2 || mips_special_table[n].type == SPECIAL_TYPE_REGS)
          {
            sprintf(temp, "%s", reg[(int)operand_reg[r]]);
          }
            else
          {
            sprintf(temp, "%d", operand_reg[r]);
          }

          if (r != 0) { strcat(instruction, ", "); }
          else { strcat(instruction, " "); }

          strcat(instruction, temp);
        }

        return 4;
      }

      n++;
    }
  }

  n = 0;
  while(mips_no_operands[n].instr != NULL)
  {
    if (mips_no_operands[n].opcode == opcode)
    {
      strcpy(instruction, mips_no_operands[n].instr);
      return 4;
    }

    n++;
  }

  n = 0;
  while(mips_other[n].instr != NULL)
  {
    if (mips_other[n].opcode == (opcode & mips_other[n].mask))
    {
      strcpy(instruction, mips_other[n].instr);

      rs = (opcode >> 21) & 0x1f;
      rt = (opcode >> 16) & 0x1f;
      rd = (opcode >> 11) & 0x1f;
      immediate = opcode & 0xffff;

      for (r = 0; r < mips_other[n].operand_count; r++)
      {
        if (r != 0) { strcat(instruction, ","); }

        switch(mips_other[n].operand[r])
        {
          case MIPS_OP_RS:
            sprintf(temp, " %s", reg[rs]);
            break;
          case MIPS_OP_RT:
            sprintf(temp, " %s", reg[rt]);
            break;
          case MIPS_OP_RD:
            sprintf(temp, " %s", reg[rd]);
            break;
          case MIPS_OP_IMMEDIATE_SIGNED:
            sprintf(temp, " %d", (int16_t)immediate);
            break;
          default:
            strcpy(temp, " ?");
            break;
        }

        strcat(instruction, temp);
      }

      return 4;
    }

    n++;
  }

  n = 0;
  while(mips_branch_table[n].instr != NULL)
  {
    if (mips_branch_table[n].op_rt == -1)
    {
      if ((opcode >> 26) == mips_branch_table[n].opcode)
      {
        rs = (opcode >> 21) & 0x1f;
        rt = (opcode >> 16) & 0x1f;
        int16_t offset = (opcode & 0xffff) << 2;

        sprintf(instruction, "%s %s, %s, 0x%x (offset=%d)", mips_branch_table[n].instr, reg[rs], reg[rt],  address + 4 + offset, offset);

        return 4;
      }
    }
      else
    {
      if ((opcode >> 26) == mips_branch_table[n].opcode &&
         ((opcode >> 16) & 0x1f) == mips_branch_table[n].op_rt)
      {
        rs = (opcode >> 21) & 0x1f;
        int16_t offset = (opcode & 0xffff) << 2;

        sprintf(instruction, "%s %s, 0x%x (offset=%d)", mips_branch_table[n].instr, reg[rs], address + 4 + offset, offset);

        return 4;
      }
    }
    n++;
  }

  if (format == 0)
  {
    // R-Type Instruction [ op 6, rs 5, rt 5, rd 5, sa 5, function 6 ]
    function = opcode & 0x3f;
    n = 0;
    while(mips_r_table[n].instr != NULL)
    {
      if (mips_r_table[n].function == function)
      {
        rs = (opcode >> 21) & 0x1f;
        rt = (opcode >> 16) & 0x1f;
        rd = (opcode >> 11) & 0x1f;
        sa = (opcode >> 6) & 0x1f;

        strcpy(instruction, mips_r_table[n].instr);

        for (r = 0; r < 3; r++)
        {
          if (mips_r_table[n].operand[r] == MIPS_OP_NONE) { break; }

          if (mips_r_table[n].operand[r] == MIPS_OP_RS)
          {
            sprintf(temp, "%s", reg[rs]);
          }
            else
          if (mips_r_table[n].operand[r] == MIPS_OP_RT)
          {
            sprintf(temp, "%s", reg[rt]);
          }
            else
          if (mips_r_table[n].operand[r] == MIPS_OP_RD)
          {
            sprintf(temp, "%s", reg[rd]);
          }
            else
          if (mips_r_table[n].operand[r] == MIPS_OP_SA)
          {
            sprintf(temp, "%d", sa);
          }
            else
          { temp[0] = 0; }

          if (r != 0) { strcat(instruction, ", "); }
          else { strcat(instruction, " "); }

          strcat(instruction, temp);
        }

        break;
      }

      n++;
    }
  }
    else
  if ((opcode >> 27) == 1)
  {
    // J-Type Instruction [ op 6, target 26 ]
    unsigned int upper = (address + 4) & 0xf0000000;
    if ((opcode >> 26) == 2)
    {
      sprintf(instruction, "j 0x%08x", ((opcode & 0x03ffffff) << 2) | upper);
    }
      else
    {
      sprintf(instruction, "jal 0x%08x", ((opcode & 0x03ffffff) << 2) | upper);
    }
  }
    else
  if ((opcode >> 28) == 4)
  {
    // Coprocessor Instruction [ op 6, format 5, ft 5, fs 5, fd 5, funct 6 ]
    function = opcode & 0x3f;
    format = (opcode >> 21) & 0x1f;
    n = 0;
    while(mips_cop_table[n].instr != NULL)
    {
      if (mips_cop_table[n].function == function &&
          mips_cop_table[n].format == format)
      {
        int ft = (opcode >> 16) & 0x1f;
        int fs = (opcode >> 11) & 0x1f;
        int fd = (opcode >> 6) & 0x1f;

        strcpy(instruction, mips_cop_table[n].instr);

        for (r = 0; r < 3; r++)
        {
          if (mips_cop_table[n].operand[r] == MIPS_COP_NONE) { break; }

          if (mips_cop_table[n].operand[r] == MIPS_COP_FD)
          {
            sprintf(temp, "$f%d", fd);
          }
            else
          if (mips_cop_table[n].operand[r] == MIPS_COP_FS)
          {
            sprintf(temp, "$f%d", fs);
          }
            else
          if (mips_cop_table[n].operand[r] == MIPS_COP_FT)
          {
            sprintf(temp, "$f%d", ft);
          }
            else
          { temp[0] = 0; }

          if (r != 0) { strcat(instruction, ", "); }
          strcat(instruction, temp);
        }

        break;
      }

      n++;
    }
  }
    else
  {
    int op = opcode >> 26;
    // I-Type?  [ op 6, rs 5, rt 5, imm 16 ]
    n = 0;
    while(mips_i_table[n].instr != NULL)
    {
      if (mips_i_table[n].function == op)
      {
        rs = (opcode >> 21) & 0x1f;
        rt = (opcode >> 16) & 0x1f;
        immediate = opcode & 0xffff;

#if 0
        if (mips_i_table[n].operand[2] == MIPS_OP_RT_IS_0)
        {
          if (rt != 0) { n++; continue; }
        }
          else
        if (mips_i_table[n].operand[2] == MIPS_OP_RT_IS_1)
        {
          if (rt != 1) { n++; continue; }
        }
#endif

        strcpy(instruction, mips_i_table[n].instr);

        for (r = 0; r < 3; r++)
        {
          if (mips_i_table[n].operand[r] == MIPS_OP_NONE) { break; }

          if (mips_i_table[n].operand[r] == MIPS_OP_RS)
          {
            sprintf(temp, "%s", reg[rs]);
          }
            else
          if (mips_i_table[n].operand[r] == MIPS_OP_RT)
          {
            sprintf(temp, "%s", reg[rt]);
          }
            else
          if (mips_i_table[n].operand[r] == MIPS_OP_IMMEDIATE)
          {
            sprintf(temp, "0x%x", immediate);
          }
            else
          if (mips_i_table[n].operand[r] == MIPS_OP_IMMEDIATE_SIGNED)
          {
            sprintf(temp, "0x%x (%d)", immediate, (int16_t)immediate);
          }
            else
          if (mips_i_table[n].operand[r] == MIPS_OP_IMMEDIATE_RS)
          {
            sprintf(temp, "0x%x(%s)", immediate, reg[rs]);
          }
            else
          if (mips_i_table[n].operand[r] == MIPS_OP_LABEL)
          {
            int32_t offset = (int16_t)immediate;

            offset = offset << 2;

            sprintf(temp, "0x%x (offset=%d)", address + 4 + offset, offset);
          }
            else
          { temp[0] = 0; }

          if (r != 0) { strcat(instruction, ", "); }
          else { strcat(instruction, " "); }
          strcat(instruction, temp);
        }

        break;
      }

      n++;
    }

    if (mips_i_table[n].instr == NULL)
    {
      //printf("Internal Error: Unknown MIPS opcode %08x, %s:%d\n", opcode, __FILE__, __LINE__);
      strcpy(instruction, "???");
    }
  }

  return 4;
}

void list_output_mips(struct _asm_context *asm_context, uint32_t start, uint32_t end)
{
  int cycles_min,cycles_max;
  char instruction[128];
  uint32_t opcode;

  fprintf(asm_context->list, "\n");

  while(start < end)
  {
    opcode = memory_read32_m(&asm_context->memory, start);

    disasm_mips(&asm_context->memory, start, instruction, &cycles_min, &cycles_max);

    fprintf(asm_context->list, "0x%08x: 0x%08x %-40s cycles: ", start, opcode, instruction);

    if (cycles_min == cycles_max)
    { fprintf(asm_context->list, "%d\n", cycles_min); }
      else
    { fprintf(asm_context->list, "%d-%d\n", cycles_min, cycles_max); }

    start += 4;
  }
}

void disasm_range_mips(struct _memory *memory, uint32_t start, uint32_t end)
{
  char instruction[128];
  int cycles_min = 0,cycles_max = 0;
  int num;

  printf("\n");

  printf("%-7s %-5s %-40s Cycles\n", "Addr", "Opcode", "Instruction");
  printf("------- ------ ----------------------------------       ------\n");

  while(start < end)
  {
    // FIXME - Need to check endian
    num = READ_RAM(start) |
          (READ_RAM(start + 1) << 8) |
          (READ_RAM(start + 2) << 16) |
          (READ_RAM(start + 3) << 24);

    disasm_mips(memory, start, instruction, &cycles_min, &cycles_max);

    if (cycles_min < 1)
    {
      printf("0x%04x: 0x%08x %-40s ?\n", start, num, instruction);
    }
      else
    if (cycles_min == cycles_max)
    {
      printf("0x%04x: 0x%08x %-40s %d\n", start, num, instruction, cycles_min);
    }
      else
    {
      printf("0x%04x: 0x%08x %-40s %d-%d\n", start, num, instruction, cycles_min, cycles_max);
    }

    start += 4;
  }
}
