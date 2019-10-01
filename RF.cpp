#ifndef _RF_H
#define _RF_H
#include "common.hpp"
#include "Register.cpp"
#include <memory>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;

enum RF_MAPPING
{
    TEMP0 = 0   ,
    TEMP1       ,
    TEMP2       ,
    TEMP3       ,
    TEMP4       ,
    USER0       , // User/Programmer registers
    USER1       ,
    USER2       ,
    USER3       ,
    USER4       ,
    USER5       ,
    USER6       ,
    USER7       ,
    USER8       ,
    USER9       ,
    USER10      ,
    USER11      ,
    USER12      ,
    USER13      , 
    USER14      ,
    USER15      ,
    USER16      ,
    USER17      ,
    USER18      ,
    USER19      ,
    INSPTR      , // Instruction pointer
    STKPTR      , // Stack pointer
    BSEPTR      , // Base pointer
    RERROR        // Register Error
};

// Register File
class RF
{
public:
    // Using a list so I can shorten the group operations I want done to them
    vector<Register> registers;
    RF()
    {
        // So I'm not driven mad by having to re-set the registers 27 lines at a time
        for (uint64_t i = 0; i < 27; i++)
        {
            registers.push_back(Register());
        }
    }
    Register& operator[](uint64_t index)
    {
        return registers[index];
    }
    string reg_to_string(uint64_t index)
    {
        switch(index)
        {
            case 0 : { return "TEMP0" ; }
            case 1 : { return "TEMP1" ; }
            case 2 : { return "TEMP2" ; }
            case 3 : { return "TEMP3" ; }
            case 4 : { return "TEMP4" ; }
            case 5 : { return "USER0" ; }
            case 6 : { return "USER1" ; }
            case 7 : { return "USER2" ; }
            case 8 : { return "USER3" ; }
            case 9 : { return "USER4" ; }
            case 10: { return "USER5" ; }
            case 11: { return "USER6" ; }
            case 12: { return "USER7" ; }
            case 13: { return "USER8" ; }
            case 14: { return "USER9" ; }
            case 15: { return "USER10"; }
            case 16: { return "USER11"; }
            case 17: { return "USER12"; }
            case 18: { return "USER13"; } 
            case 19: { return "USER14"; }
            case 20: { return "USER15"; }
            case 21: { return "USER16"; }
            case 22: { return "USER17"; }
            case 23: { return "USER18"; }
            case 24: { return "USER19"; }
            case 25: { return "INSPTR"; } // Instruction pointer
            case 26: { return "STKPTR"; } // Stack pointer
            case 27: { return "BSEPTR"; } // Base pointer
            default: { return std::to_string(index); }
        }
    }
    uint64_t string_to_reg(string reg)
    {
        if (reg == "TEMP0" ) { return TEMP0 ; }
        if (reg == "TEMP1" ) { return TEMP1 ; }
        if (reg == "TEMP2" ) { return TEMP2 ; }
        if (reg == "TEMP3" ) { return TEMP3 ; }
        if (reg == "TEMP4" ) { return TEMP4 ; }
        if (reg == "USER0" ) { return USER0 ; }
        if (reg == "USER1" ) { return USER1 ; }
        if (reg == "USER2" ) { return USER2 ; }
        if (reg == "USER3" ) { return USER3 ; }
        if (reg == "USER4" ) { return USER4 ; }
        if (reg == "USER5" ) { return USER5 ; }
        if (reg == "USER6" ) { return USER6 ; }
        if (reg == "USER7" ) { return USER7 ; }
        if (reg == "USER8" ) { return USER8 ; }
        if (reg == "USER9" ) { return USER9 ; }
        if (reg == "USER10") { return USER10; }
        if (reg == "USER11") { return USER11; }
        if (reg == "USER12") { return USER12; }
        if (reg == "USER13") { return USER13; } 
        if (reg == "USER14") { return USER14; }
        if (reg == "USER15") { return USER15; }
        if (reg == "USER16") { return USER16; }
        if (reg == "USER17") { return USER17; }
        if (reg == "USER18") { return USER18; }
        if (reg == "USER19") { return USER19; }
        if (reg == "INSPTR") { return INSPTR; } // Instruction pointer
        if (reg == "STKPTR") { return STKPTR; } // Stack pointer
        if (reg == "BSEPTR") { return BSEPTR; } // Base pointer
        return RERROR;
    }
    string setw(string str, uint64_t n)
    {
        string sum = "";
        for (uint64_t i = 0; i < n; i++)
        {
            sum += str;
        }
        return sum;
    }
    void print()
    {
        for (uint64_t i = 0; i < registers.size(); i++)
        {
            string reg = reg_to_string(i);
            int64_t extra_spaces = 6 - reg.length();
            cout << reg << setw(" ", extra_spaces) << ": " << registers[i].reg << endl;
        }
    }
};
#endif