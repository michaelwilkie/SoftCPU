#ifndef _REGISTER_H
#define _REGISTER_H

#include <iostream>

using std::ostream;

class Register
{
public:
    uint64_t reg;
    Register()
    {
        reg = 0;
    }
    uint64_t operator=(Register &val) { return reg = val.reg ; }
    uint64_t operator=(uint64_t val)  { return reg = val     ; }

    uint64_t operator+(Register &val) { return reg + val.reg ; }
    uint64_t operator+(uint64_t val)  { return reg + val     ; }

    uint64_t operator-(Register &val) { return reg - val.reg ; }
    uint64_t operator-(uint64_t val)  { return reg - val     ; }

    uint64_t operator*(Register &val) { return reg * val.reg ; }
    uint64_t operator*(uint64_t val)  { return reg * val     ; }

    uint64_t operator/(Register &val) { return reg / val.reg ; }
    uint64_t operator/(uint64_t val)  { return reg / val     ; }

    uint64_t operator&(Register &val) { return reg & val.reg ; }
    uint64_t operator&(uint64_t val)  { return reg & val     ; }

    uint64_t operator^(Register &val) { return reg ^ val.reg ; }
    uint64_t operator^(uint64_t val)  { return reg ^ val     ; }

    uint64_t operator|(Register &val) { return reg | val.reg ; }
    uint64_t operator|(uint64_t val)  { return reg | val     ; }
};

#endif