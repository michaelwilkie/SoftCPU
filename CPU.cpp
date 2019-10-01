#ifndef _CPU_H
#define _CPU_H
#include "common.hpp"

// defined in RF.cpp

enum CPU_INSTRUCTION_SET
{
    INSTR_HALT=0 ,   // stops the processor from running any more commands
    INSTR_ADD    ,   // add     a    ,   b    add a and b
    INSTR_SUB    ,   // sub     a    ,   b    subtract a and b
    INSTR_MULT   ,   // mult    a    ,   b    multiply a and b
    INSTR_DIV    ,   // div     a    ,   b    divide a and b
    INSTR_AND    ,   // and     a    ,   b    apply and operator to a and b
    INSTR_OR     ,   // or      a    ,   b    apply or operator to a and b
    INSTR_XOR    ,   // xor     a    ,   b    apply xor operator to a and b
    INSTR_INC    ,   // inc     a             increment value of a by 1
    INSTR_DEC    ,   // dec     a             decrement value of a by 1
    INSTR_CMP    ,   // cmp     a    ,   b    compare a and b, result stored in CP_FL
    INSTR_CALL   ,   // call    i             puts instruction pointer + 1 on stack, then jumps to address i
    INSTR_RET    ,   // ret                   resets instruction pointer to value on top of the stack
    INSTR_PUSH   ,   // push    a             puts the value in a on the stack
    INSTR_POP    ,   // pop     a             removes the value on the stack and puts it into a
    INSTR_JMP    ,   // jmp     a             sets instruction pointer to a
    INSTR_JE     ,   // je      a             sets instruction pointer to a if CP_FL is 0
    INSTR_JNE    ,   // jne     a             sets instruction pointer to a if CP_FL is not 0
    INSTR_JL     ,   // jl      a             sets instruction pointer to a if CP_FL is less than 0
    INSTR_JLE    ,   // jle     a             sets instruction pointer to a if CP_FL is less than or equal to 0
    INSTR_JG     ,   // jl      a             sets instruction pointer to a if CP_FL is greater than 0
    INSTR_JGE    ,   // jge     a             sets instruction pointer to a if CP_FL is greater than or equal to 0
    INSTR_MOV    ,   // mov     a    ,   b    undefined, will do later
    INSTR_IRMOV  ,   // irmov   a    ,   i    move the value of i to a
    INSTR_IMMOV  ,   // immov   i1   ,   i2   move the value of i2 to the address of i1
    INSTR_RRMOV  ,   // rrmov   a    ,   b    move value of b to a
    INSTR_MRMOV  ,   // mrmov   a    ,   i    move the value in the address of i to a
    INSTR_RMMOV  ,   // rmmov   a    ,   i    move the value of a to the address of i
    INSTR_MMMOV  ,   // mmmov   i1   ,   i2   move the value in the address i2 into the address of i1
    INSTR_ERROR      // bad instruction
};

// Central Processing Unit
class CPU
{
public:
    ALU CPU_ALU     ;  // Arithmetic Logic Unit
    uint64_t OF_FL  ;  // Overflow flag
    int64_t CP_FL   ;  // Comparison flag

    uint64_t func   ;  // deciphers the operator block of an instruction
    uint64_t valA   ;  // first argument in an operation
    uint64_t valB   ;  // second argument in an operation

    uint64_t halted ;

    RF CPU_RF       ;  // Register file
    Memory CPU_Memory; // Main Memory
    CPU()
    {
        halted = 0;
        CPU_RF[STKPTR] = CPU_Memory.memsize - 1;
        cout << "Stack pointer: " << CPU_RF[STKPTR].reg << endl;
    }


    // I wanted indexes to be 8 bits, but that will complicate how I will read these data
    // from memory, which is going to be a file, so I stuck with 64 bit integers
    uint64_t add(uint64_t index, uint64_t index2)
    {
        return CPU_RF[index] = CPU_ALU.int64_int64_add(CPU_RF[index], CPU_RF[index2]);
    }
    uint64_t sub(uint64_t index, uint64_t index2)
    {
        return CPU_RF[index] = CPU_ALU.int64_int64_sub(CPU_RF[index], CPU_RF[index2]);
    }
    uint64_t mult(uint64_t index, uint64_t index2)
    {
        return CPU_RF[index] = CPU_ALU.int64_int64_mult(CPU_RF[index], CPU_RF[index2]);
    }
    uint64_t div(uint64_t index, uint64_t index2)
    {
        return CPU_RF[index] = CPU_ALU.int64_int64_div(CPU_RF[index], CPU_RF[index2]);
    }
    uint64_t _and(uint64_t index, uint64_t index2)
    {
        return CPU_RF[index] = CPU_ALU.int64_int64_and(CPU_RF[index], CPU_RF[index2]);
    }
    uint64_t _or(uint64_t index, uint64_t index2)
    {
        return CPU_RF[index] = CPU_ALU.int64_int64_or(CPU_RF[index], CPU_RF[index2]);
    }
    uint64_t _xor(uint64_t index, uint64_t index2)
    {
        return CPU_RF[index] = CPU_ALU.int64_int64_xor(CPU_RF[index], CPU_RF[index2]);
    }
    uint64_t inc(uint64_t index)
    {
        return CPU_RF[index] = CPU_ALU.int64_int64_inc(CPU_RF[index]);
    }
    uint64_t dec(uint64_t index)
    {
        return CPU_RF[index] = CPU_ALU.int64_int64_dec(CPU_RF[index]);
    }
    uint64_t cmp(uint64_t index, uint64_t index2)
    {
        return CP_FL = CPU_RF[index] - CPU_RF[index2];
    }
    uint64_t call(uint64_t addr)
    {
        CPU_RF[STKPTR] = CPU_RF[STKPTR] - 1;
        CPU_Memory.write(CPU_RF[STKPTR].reg, CPU_RF[INSPTR].reg + 1);
        
        return CPU_RF[INSPTR] = addr;
    }
    uint64_t ret()
    {
        CPU_RF[INSPTR] = CPU_Memory[CPU_RF[STKPTR].reg];
        CPU_RF[STKPTR] = CPU_RF[STKPTR] + 1;

        return CPU_RF[INSPTR].reg;
    }
    uint64_t push(uint64_t index)
    {
        CPU_RF[STKPTR] = CPU_RF[STKPTR] - 1;
        CPU_Memory.write(CPU_RF[STKPTR].reg, CPU_RF[index].reg);

        return CPU_RF[index].reg;
    }
    uint64_t pop(uint64_t index)
    {
        CPU_RF[index] = CPU_Memory[CPU_RF[STKPTR].reg];
        CPU_RF[STKPTR] = CPU_RF[STKPTR] + 1;
        
        return CPU_RF[index].reg;
    }
    uint64_t jmp(uint64_t address)
    {
        return CPU_RF[INSPTR] = address;
    }
    uint64_t je(uint64_t address)
    {
        if (!CP_FL)
        {
            return CPU_RF[INSPTR] = address;
        }
        return CPU_RF[INSPTR].reg + 1;
    }
    uint64_t jne(uint64_t address)
    {
        if (CP_FL)
        {
            return CPU_RF[INSPTR] = address;
        }
        return CPU_RF[INSPTR].reg + 1;
    }
    uint64_t jl(uint64_t address)
    {
        if (CP_FL < 0)
        {
            return CPU_RF[INSPTR] = address;
        }
        return CPU_RF[INSPTR].reg + 1;
    }
    uint64_t jle(uint64_t address)
    {
        if (CP_FL <= 0)
        {
            return CPU_RF[INSPTR] = address;
        }
        return CPU_RF[INSPTR].reg + 1;
    }
    uint64_t jg(uint64_t address)
    {
        if (CP_FL > 0)
        {
            return CPU_RF[INSPTR] = address;
        }
        return CPU_RF[INSPTR].reg + 1;
    }
    uint64_t jge(uint64_t address)
    {
        if (CP_FL >= 0)
        {
            return CPU_RF[INSPTR] = address;
        }
        return CPU_RF[INSPTR].reg + 1;
    }
    uint64_t irmov(uint64_t index, uint64_t val)
    {
        return CPU_RF[index] = val;
    }
    void rrmov(uint64_t index, uint64_t index2)
    {
        CPU_RF[index] = CPU_RF[index2];
    }
    void rmmov(uint64_t index, uint64_t address)
    {
        CPU_Memory.write(address, CPU_RF[index].reg);
    }
    void mrmov(uint64_t index, uint64_t address)
    {
        CPU_RF[index] = CPU_Memory[address];
    }
    void immov(uint64_t address, uint64_t val)
    {
        CPU_Memory.write(address, val);
    }
    void mmmov(uint64_t address1, uint64_t address2)
    {
        CPU_RF[TEMP0] = CPU_Memory[address1];
        CPU_RF[TEMP1] = CPU_Memory[address2];
        CPU_Memory.write(address2, CPU_RF[TEMP0].reg);
        CPU_Memory.write(address1, CPU_RF[TEMP1].reg);
    }
    uint64_t peek(uint64_t address)
    {
        return CPU_Memory[address];
    }
    // restores a halted processor to begin executing at the given address
    void reset(uint64_t addr)
    {
        if (halted)
        {
            CPU_RF[INSPTR] = addr;
            halted = 0;
        }
    }
    void execute()
    {
        if (!halted)
        {
            func = CPU_Memory[CPU_RF[INSPTR].reg];
            CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

            cout << instruction_to_string(func) << endl;
            switch(func)
            {
                case INSTR_HALT:
                {
                    cout << "Halted" << endl;
                    halted = 1;
                    break;
                }
                case INSTR_ADD:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    add(valA, valB);

                    break;
                }
                case INSTR_SUB:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    sub(valA, valB);

                    break;
                }
                case INSTR_MULT:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    mult(valA, valB);

                    break;
                }
                case INSTR_DIV:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    div(valA, valB);

                    break;
                }
                case INSTR_AND:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    _and(valA, valB);

                    break;
                }
                case INSTR_OR:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    _or(valA, valB);

                    break;
                }
                case INSTR_XOR:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    _xor(valA, valB);

                    break;
                }
                case INSTR_INC:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    inc(valA);

                    break;
                }
                case INSTR_DEC:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    dec(valA);

                    break;
                }
                case INSTR_CMP:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    cmp(valA, valB);

                    break;
                }
                case INSTR_CALL:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];            
                    CPU_RF[INSPTR] = call(valA);

                    break;
                }
                case INSTR_RET:
                {
                    ret();

                    break;
                }
                case INSTR_PUSH:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    push(valA);
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    break;
                }
                case INSTR_POP:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    pop(valA);
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    break;
                }
                case INSTR_JMP:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = jmp(valA);

                    break;
                }
                case INSTR_JE:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = je(valA);

                    break;
                }
                case INSTR_JNE:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = jne(valA);

                    break;
                }
                case INSTR_JL:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = jl(valA); // jl takes care of the CP_FL checking

                    break;
                }
                case INSTR_JLE:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = jle(valA); // jle takes care of the CP_FL checking

                    break;
                }
                case INSTR_JG:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = jg(valA); // jg takes care of the CP_FL checking

                    break;
                }
                case INSTR_JGE:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = jge(valA); // jge takes care of the CP_FL checking

                    break;
                }
                case INSTR_MOV:
                {
                    break; // will do later
                }
                case INSTR_IRMOV:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    irmov(valA, valB);
        
                    break;
                }
                case INSTR_IMMOV:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    immov(valA, valB);
        
                    break;
                }
                case INSTR_RRMOV:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    rrmov(valA, valB);

                    break;
                }
                case INSTR_MRMOV:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    mrmov(valA, valB);

                    break;
                }
                case INSTR_RMMOV:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    mmmov(valA, valB);

                    break;
                }
                case INSTR_MMMOV:
                {
                    valA = CPU_Memory[CPU_RF[INSPTR].reg + 1];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    valB = CPU_Memory[CPU_RF[INSPTR].reg + 2];
                    CPU_RF[INSPTR] = CPU_RF[INSPTR] + 1;

                    mmmov(valA, valB);
                    
                    break;
                }
                default:
                {
                    cout << "Illegal instruction: " << func << endl;
                    cout << "Halting..." << endl;
                    halted = 1;
                }
            }
        }
        else 
        {
            cout << "Processor halted" << endl;
        }
    }
    string instruction_to_string(uint64_t instr)
    {
        switch(instr)
        {
            case INSTR_HALT : { return "halt"   ;}
            case INSTR_ADD  : { return "add"    ;}
            case INSTR_SUB  : { return "sub"    ;}
            case INSTR_MULT : { return "mult"   ;}
            case INSTR_DIV  : { return "div"    ;}
            case INSTR_AND  : { return "and"    ;}
            case INSTR_OR   : { return "or"     ;}
            case INSTR_XOR  : { return "xor"    ;}
            case INSTR_INC  : { return "inc"    ;}
            case INSTR_DEC  : { return "dec"    ;}
            case INSTR_CMP  : { return "cmp"    ;}
            case INSTR_CALL : { return "call"   ;}
            case INSTR_RET  : { return "ret"    ;}
            case INSTR_PUSH : { return "push"   ;}
            case INSTR_POP  : { return "pop"    ;}
            case INSTR_JMP  : { return "jmp"    ;}
            case INSTR_JE   : { return "je"     ;}
            case INSTR_JNE  : { return "jne"    ;}
            case INSTR_JL   : { return "jl"     ;}
            case INSTR_JLE  : { return "jle"    ;}
            case INSTR_JG   : { return "jg"     ;}
            case INSTR_JGE  : { return "jge"    ;}
            case INSTR_MOV  : { return "mov"    ;}
            case INSTR_IRMOV: { return "irmov"  ;}
            case INSTR_IMMOV: { return "immov"  ;}
            case INSTR_RRMOV: { return "rrmov"  ;}
            case INSTR_MRMOV: { return "mrmov"  ;}
            case INSTR_RMMOV: { return "rmmov"  ;}
            case INSTR_MMMOV: { return "mmmov"  ;}
            default:          { return "bad instruction";}
        }
    }
    uint64_t string_to_instruction(string instr)
    {
        if (instr == "halt"   ) { return INSTR_HALT  ; }
        if (instr == "add"    ) { return INSTR_ADD   ; }
        if (instr == "sub"    ) { return INSTR_SUB   ; }
        if (instr == "mult"   ) { return INSTR_MULT  ; }
        if (instr == "div"    ) { return INSTR_DIV   ; }
        if (instr == "inc"    ) { return INSTR_INC   ; }
        if (instr == "dec"    ) { return INSTR_DEC   ; }
        if (instr == "and"    ) { return INSTR_AND   ; }
        if (instr == "or"     ) { return INSTR_OR    ; }
        if (instr == "xor"    ) { return INSTR_XOR   ; }
        if (instr == "cmp"    ) { return INSTR_CMP   ; }
        if (instr == "call"   ) { return INSTR_CALL  ; }
        if (instr == "ret"    ) { return INSTR_RET   ; }
        if (instr == "push"   ) { return INSTR_PUSH  ; }
        if (instr == "pop"    ) { return INSTR_POP   ; }
        if (instr == "jmp"    ) { return INSTR_JMP   ; }
        if (instr == "je"     ) { return INSTR_JE    ; }
        if (instr == "jne"    ) { return INSTR_JNE   ; }
        if (instr == "jl"     ) { return INSTR_JL    ; }
        if (instr == "jle"    ) { return INSTR_JLE   ; }
        if (instr == "jg"     ) { return INSTR_JG    ; }
        if (instr == "jge"    ) { return INSTR_JGE   ; }
        if (instr == "mov"    ) { return INSTR_MOV   ; }
        if (instr == "irmov"  ) { return INSTR_IRMOV ; }
        if (instr == "immov"  ) { return INSTR_IMMOV ; }
        if (instr == "rrmov"  ) { return INSTR_RRMOV ; }
        if (instr == "mrmov"  ) { return INSTR_MRMOV ; }
        if (instr == "rmmov"  ) { return INSTR_RMMOV ; }
        if (instr == "mmmov"  ) { return INSTR_MMMOV ; }
        return INSTR_ERROR;
        
    }
private:
    uint64_t rd(uint64_t index, uint64_t address)
    {
        return CPU_RF[index] = CPU_Memory[address];
    }
    void wr(uint64_t index, uint64_t address)
    {
        uint64_t val = CPU_Memory[address];
        
        CPU_Memory.write(address, CPU_RF[index].reg);
    }    
};
#endif
