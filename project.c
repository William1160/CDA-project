#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if(Mem == NULL || instruction == NULL) {
        return 1;
    }
    if (PC % 4 != 0)
    {
        return 1;
    }
    unsigned index = PC >> 2;
    if (PC > 16383)
    {
        return 1;
    }
    *instruction = Mem[index];
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    
    *op =     (instruction & 0xfc000000) >> 26;        // instructions [31-26]        1111 1100 0000 0000 0000 0000 0000 0000
    *r1 =     (instruction & 0x03e00000) >> 21;        // instructions [25-21]        0000 0011 1110 0000 0000 0000 0000 0000
    *r2 =     (instruction & 0x001f0000) >> 16;        // instructions [20-16]        0000 0000 0001 1111 0000 0000 0000 0000
    *r3 =     (instruction & 0x0000f800) >> 11;        // instructions [15-11]        0000 0000 0000 0000 1111 1000 0000 0000
    *funct =  (instruction & 0x0000003f);              // instructions [5-0]          0000 0000 0000 0000 0000 0000 0011 1111
    *offset = (instruction & 0x0000ffff);              // instructions [15-0]         0000 0000 0000 0000 1111 1111 1111 1111
    *jsec =   (instruction & 0x003fffff);              // instructions [25-0]         0000 0011 1111 1111 1111 1111 1111 1111

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

// ./cisproject/spimcore cisproject/input_file.asc
