#ifndef _ALU_H
#define _ALU_H
#include "common.hpp"
#include "Register.cpp"
// Arithmetic Logic Unit
class ALU
{
public:
    ALU() {}
    uint64_t int64_int64_add(Register a, Register b)
    {
        return a + b;
    }
    uint64_t int64_int64_sub(Register a, Register b)
    {
        return a - b;
    }
    uint64_t int64_int64_mult(Register a, Register b)
    {
        return a * b;
    }
    uint64_t int64_int64_div(Register a, Register b)
    {
        return a / b;
    }
    uint64_t int64_int64_inc(Register a)
    {
        return a + 1;
    }
    uint64_t int64_int64_dec(Register a)
    {
        return a - 1;
    }
    uint64_t int64_int64_and(Register a, Register b)
    {
        return a & b;
    }
    uint64_t int64_int64_or(Register a, Register b)
    {
        return a | b;
    }
    uint64_t int64_int64_xor(Register a, Register b)
    {
        return a ^ b;
    }
};
#endif