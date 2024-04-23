#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch(ALUControl) {
        case 0x0:
            *ALUresult = A + B;
            break;
        case 0x1:
            *ALUresult = A - B;
            break;
        case 0x2:
            *ALUresult = ((int)A < (int)B) ? 1 : 0; // signed, hence (int) to cast
            break;
        case 0x3:
            *ALUresult = (A < B) ? 1 : 0; // unsigned, no casting needed
            break;
        case 0x4:
            *ALUresult = A & B;
            break;
        case 0x5:
            *ALUresult = A | B;
            break;
        case 0x6:
            *ALUresult = B << 16;
            break;
        case 0x7:
            *ALUresult = ~A;
            break;
    }

    if(ALUresult == 0)
        *Zero = 1;
    else
        *Zero = 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if(Mem == NULL || instruction == NULL){
        return 1;
    }
    if (PC % 4 != 0)
    {
        return 1;
    }
    unsigned index = PC >> 2;
    if (index > 16383)
    {
        return 1;
    }
    *instruction = Mem[index];
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

    *op = (instruction & 0xfc000000) >> 26; // instructions [31-26]        1111 1100 0000 0000 0000 0000 0000 0000
    *r1 = (instruction & 0x03e00000) >> 21; // instructions [25-21]        0000 0011 1110 0000 0000 0000 0000 0000
    *r2 = (instruction & 0x001f0000) >> 16; // instructions [20-16]        0000 0000 0001 1111 0000 0000 0000 0000
    *r3 = (instruction & 0x0000f800) >> 11; // instructions [15-11]        0000 0000 0000 0000 1111 1000 0000 0000
    *funct = (instruction & 0x0000003f);    // instructions [5-0]          0000 0000 0000 0000 0000 0000 0011 1111
    *offset = (instruction & 0x0000ffff);   // instructions [15-0]         0000 0000 0000 0000 1111 1111 1111 1111
    *jsec = (instruction & 0x003fffff);     // instructions [25-0]         0000 0011 1111 1111 1111 1111 1111 1111
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{
    switch (op)
    {
    case 0x00: // R-type
        controls->RegDst = '1';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '7';
        controls->MemWrite = '0';
        controls->ALUSrc = '0';
        controls->RegWrite = '1';
        break;
    case 0x02: // jump
        controls->RegDst = '0';
        controls->Jump = '1';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '0';
        controls->MemWrite = '0';
        controls->ALUSrc = '0';
        controls->RegWrite = '1';
        break;
    case 0x04: // beq
        controls->RegDst = '2';
        controls->Jump = '0';
        controls->Branch = '1';
        controls->MemRead = '0';
        controls->MemtoReg = '2';
        controls->ALUOp = '1';
        controls->MemWrite = '0';
        controls->ALUSrc = '2';
        controls->RegWrite = '0';
        break;
    case 0x08: // addi
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '0';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
        break;
    case 0x0a: // slti
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '2';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
        break;
    case 0x0b: // sltiu
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '3';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
        break;
    case 0x0f: // lui
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '6';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
        break;
    case 0x23: // lw
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '1';
        controls->MemtoReg = '1';
        controls->ALUOp = '0';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
        break;
    case 0x2b: // sw
        controls->RegDst = '2';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '2';
        controls->ALUOp = '0';
        controls->MemWrite = '1';
        controls->ALUSrc = '1';
        controls->RegWrite = '0';
        break;
    default:
        return 1;
    }

    return 0;
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
    if(offset >> 15 == 1)
        *extended_value = offset | 0xffff0000; // since it's negative you add 1's to the front
    else
        *extended_value = offset & 0x0000ffff; // is positive so add 0's to front
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    if(ALUOp <= 0 || ALUOp > 7)
        return 1;
    
    if(ALUOp == 7) {
        switch(funct) {
            case 0x20: // add
                ALUControl = 0;
                break;
            case 0x22: // sub
                ALUControl = 1;
                break;
            case 0x24: // and
                ALUControl = 4;
                break;
            case 0x25: // or
                ALUControl = 5;
                break;
            case 0x2a: // slt
                ALUControl = 2;
                break;
            case 0x2b: // sltu
                ALUControl = 3;
                break;
            default:
                return 1;
        }

        if(ALUSrc == 1)
            ALU(data1, extended_value, ALUOp, ALUresult, Zero);
        else
            ALU(data1, data2, ALUOp, ALUresult, Zero);

        return 0;
    }

    
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    if(MemRead == 1) {
        if(ALUresult % 4 != 0)
            return 1;
        *memdata = Mem[ALUresult >> 2];
    }
    
    if(MemWrite == 1) {
        if(ALUresult % 4 != 0)
            return 1;
        Mem[ALUresult >> 2] = data2;
    }

    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite == '1') {
        if(MemtoReg == '1') {
            if (RegDst == '1')
            {
                Reg[r3] = memdata;
            }
            else
            {
                Reg[r2] = memdata;
            }
        }
        else {
            if(RegDst == '1') {
                Reg[r3] = ALUresult;
            }
            else {
                Reg[r2] = ALUresult;
            }
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

void WinMain() {

}
// ./cisproject/spimcore cisproject/input_file.asc
