#include <iostream>
#include "CPU.cpp"
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>

#define CLEAR_SCREEN (cout << "\033[2J\033[1;1H")

using namespace std;

void compile(ifstream &file, CPU &processor);
void interpret_at(CPU &processor);
vector<string> splitStringtoVector(string str);
void write_instruction(vector<string> &tokens, CPU &processor, uint64_t &ptr);

int main() 
{
    CLEAR_SCREEN;

    CPU processor;
    
    ifstream file("program");

    if (!file.good())
    {
        cout << "Could not open program file. Exiting..." << endl;
        return 0;
    }

    compile(file, processor);

    cout << "--------------"   << endl;
    cout << "Assembly:"        << endl;
    interpret_at(processor);
    cout << "--------------"   << endl;
    cout << "Executing..."     << endl;
    while(!processor.halted)
        processor.execute();    
    cout << "--------------"   << endl;
    cout << "Result: "         << endl;
    processor.CPU_RF.print();
    cout << "--------------"   << endl;
    cout << "Program finished" << endl;

    return 0;
}
void compile(ifstream &file, CPU &processor)
{
    vector<string> tokens;
    string line;

    // flatten the entire file into a sequence of string tokens
    while (getline(file, line))
    {
        vector<string> arr = splitStringtoVector(line);
        for (string e: arr)
        {
            tokens.push_back(e);
        }
    }

    for (uint64_t i = 0; i < tokens.size();/*i is modified inside the for loop body*/)
    {
        write_instruction(tokens, processor, i);
    }

}
vector<string> splitStringtoVector(string str)
{
    stringstream ss(str);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> vstrings(begin, end);
    return vstrings;
}
void writeHelloWorld(CPU &processor)
{
    uint16_t addr = 0;
    processor.CPU_Memory.write(addr + 0, 'h');
    processor.CPU_Memory.write(addr + 1, 'e');
    processor.CPU_Memory.write(addr + 2, 'l');
    processor.CPU_Memory.write(addr + 3, 'l');
    processor.CPU_Memory.write(addr + 4, 'o');
    processor.CPU_Memory.write(addr + 5, ' ');
    processor.CPU_Memory.write(addr + 6, 'w');
    processor.CPU_Memory.write(addr + 7, 'o');
    processor.CPU_Memory.write(addr + 8, 'r');
    processor.CPU_Memory.write(addr + 9, 'l');    
    processor.CPU_Memory.write(addr +10, 'd');
    processor.CPU_Memory.write(addr +11, '\0');
}
void write_instruction(vector<string> &tokens, CPU &processor, uint64_t &ptr)
{
    uint64_t current_instr = processor.string_to_instruction(tokens[ptr]);

    uint64_t valA = 0 - 1; 
    uint64_t valB = 0 - 1;

    if (ptr + 1 >= tokens.size())
    {
        ptr++;
        return;
    }
    valA = processor.CPU_RF.string_to_reg(tokens[ptr+1]);
    if (ptr + 2 >= tokens.size())
    {
        uint64_t valB = 1337;
    }
    switch(current_instr)    
    {
        case INSTR_HALT:
        {
            processor.immov(ptr, current_instr);
            ptr++;
            return;
        }
        case INSTR_ADD:
        {
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }            
            valB = processor.CPU_RF.string_to_reg(tokens[ptr+2]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            ptr += 3;

            break;
        }
        case INSTR_SUB:
        {
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }
            valB = processor.CPU_RF.string_to_reg(tokens[ptr+2]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            ptr += 3;

            break;
        }
        case INSTR_MULT:
        {       
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }         
            valB = processor.CPU_RF.string_to_reg(tokens[ptr+2]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            ptr += 3;

            break;
        }
        case INSTR_DIV:
        {        
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }        
            valB = processor.CPU_RF.string_to_reg(tokens[ptr+2]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            ptr += 3;

            break;
        }
        case INSTR_INC:
        {                
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;

            break;
        }
        case INSTR_DEC:
        {
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
            
            break;
        }
        
        case INSTR_CMP:
        {
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }
            valB = processor.CPU_RF.string_to_reg(tokens[ptr+2]);
            
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            ptr += 3;

            break;
        }
        case INSTR_CALL:
        {
            valA = stoi(tokens[ptr+1]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
            
            break;
        }
        case INSTR_RET:
        {
            processor.immov(ptr, current_instr);
            ptr++;
            
            break;
        }
        case INSTR_PUSH:
        {
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
            
            break;
        }
        case INSTR_POP:
        {
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
            
            break;
        }
        case INSTR_JMP:
        {                
            valA = stoi(tokens[ptr+1]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
            
            break;
        }
        case INSTR_JE:
        {
            valA = stoi(tokens[ptr+1]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
                         
            break;
        }
        case INSTR_JNE:
        {
            valA = stoi(tokens[ptr+1]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
                           
            break;
        }
        case INSTR_JL:
        {
            valA = stoi(tokens[ptr+1]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
                          
            break;
        }
        case INSTR_JLE:
        {
            valA = stoi(tokens[ptr+1]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
                          
            break;
        }
        case INSTR_JG:
        {
            valA = stoi(tokens[ptr+1]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
                           
            break;
        }
        case INSTR_JGE:
        {
            valA = stoi(tokens[ptr+1]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            ptr += 2;
                           
            break;
        }
        case INSTR_MOV:
        {
            break; // will do later
        }
        case INSTR_IRMOV:
        {
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }
            valB = (uint64_t)stoi(tokens[ptr+2]);

            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);

            ptr += 3;

            break;
        }
        case INSTR_IMMOV:
        {
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }
            valB = (uint64_t)stoi(tokens[ptr+2]);
            
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            
            ptr += 3;

            break;
        }
        case INSTR_RRMOV:
        {
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }
            valB = processor.CPU_RF.string_to_reg(tokens[ptr+2]);
            
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            ptr += 3;

            break;
        }
        case INSTR_MRMOV:
        {
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }
            valB = processor.CPU_RF.string_to_reg(tokens[ptr+2]);
            
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            ptr += 3;

            break;
        }
        case INSTR_RMMOV:
        {
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }
            valB = processor.CPU_RF.string_to_reg(tokens[ptr+2]);
            
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            ptr += 3;

            break;
        }
        case INSTR_MMMOV:
        {
            if (valB == 1337)
            {
                cout << "Error: " << tokens[ptr] << endl << "    Not enough parameters" << endl;
                ptr++;
                break;
            }
            valB = processor.CPU_RF.string_to_reg(tokens[ptr+2]);
            
            processor.immov(ptr, current_instr);
            processor.immov(ptr + 1, valA);
            processor.immov(ptr + 2, valB);
            ptr += 3;

            break;
        }
        default:
        {
            cout << "Bad instruction: " << tokens[ptr] << endl;
            exit(0);
        }
    }        
}
void interpret_at(CPU &processor)
{
    uint64_t ptr = 0;
    uint64_t current_instr = ptr;
    uint64_t stopprinting = 0;
    while (ptr < 2000)
    {
        current_instr = ptr;
        ptr++;
        switch(processor.CPU_Memory[current_instr])
        {
            case INSTR_HALT:
            {
                if (!stopprinting)
                    cout << current_instr << " halt" << endl;
                stopprinting = 1;
                break;
            }
            case INSTR_ADD:
            {
                cout << current_instr << " add " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                        << " "  << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr + 1]) 
                                << endl;
                ptr += 2;
                stopprinting = 0;
                break;
            }
            case INSTR_SUB:
            {
                cout << current_instr << " sub " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                        << " "  << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr + 1]) 
                                << endl;
                ptr += 2;
                stopprinting = 0;
                break;
            }
            case INSTR_MULT:
            {                
                cout << current_instr << " mult " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                        << " "   << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr + 1]) 
                                 << endl;
                ptr += 2;
                stopprinting = 0;
                break;
            }
            case INSTR_DIV:
            {                
                cout << current_instr << " div " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                        << " "   << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr + 1]) 
                                 << endl;
                ptr += 2;
                stopprinting = 0;
                break;
            }
            case INSTR_INC:
            {                
                cout << current_instr << " inc " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                                      << endl;
                ptr += 1;
                stopprinting = 0;
                break;
            }
            case INSTR_DEC:
            {                
                cout << current_instr << " dec " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr])
                                << endl;
                ptr += 1;
                stopprinting = 0;                 
                break;
            }
            
            case INSTR_CMP:
            {
                cout << current_instr << " cmp " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                        << " "   << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr + 1]) 
                                 << endl;
                ptr += 2;
                stopprinting = 0;                 
                break;
            }
            case INSTR_CALL:
            {
                cout << current_instr << " call " << processor.CPU_Memory[ptr]
                                 << endl;
                ptr++;
                stopprinting = 0;                 
                break;
            }
            case INSTR_RET:
            {
                cout << current_instr << " ret " << endl;
                ptr++;
                stopprinting = 0;                 
                break;
            }
            case INSTR_PUSH:
            {
                cout << current_instr << " push " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                                << endl;
                ptr++;
                stopprinting = 0;                 
                break;
            }
            case INSTR_POP:
            {
                cout << current_instr << " pop " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                                << endl;
                ptr++;
                stopprinting = 0;                 
                break;
            }
            case INSTR_JMP:
            {                
                cout << current_instr << " jmp " << processor.CPU_Memory[ptr] 
                     << endl;
                ptr++;
                stopprinting = 0;                 
                break;
            }
            case INSTR_JE:
            {
                cout << current_instr << " je " << processor.CPU_Memory[ptr] 
                        << endl;
                ptr++;                
                stopprinting = 0;                 
                break;
            }
            case INSTR_JNE:
            {
                cout << current_instr << " jne " << processor.CPU_Memory[ptr] 
                        << endl;
                ptr++;                
                stopprinting = 0;                 
                break;
            }
            case INSTR_JL:
            {
                cout << current_instr << " jl " << processor.CPU_Memory[ptr] 
                        << endl;
                ptr++;                
                stopprinting = 0;                 
                break;
            }
            case INSTR_JLE:
            {
                cout << current_instr << " jle " << processor.CPU_Memory[ptr] 
                        << endl;
                ptr++;                
                stopprinting = 0;                 
                break;
            }
            case INSTR_JG:
            {
                cout << current_instr << " jg " << processor.CPU_Memory[ptr] 
                        << endl;
                ptr++;                
                stopprinting = 0;                 
                break;
            }
            case INSTR_JGE:
            {
                cout << current_instr << " jge " << processor.CPU_Memory[ptr] 
                        << endl;
                ptr++;                
                stopprinting = 0;                 
                break;
            }
            case INSTR_MOV:
            {
                stopprinting = 0;                 
                break; // will do later
            }
            case INSTR_IRMOV:
            {                
                cout << current_instr << " irmov " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                        << " "    << processor.CPU_Memory[ptr + 1] 
                                  << endl;
                ptr += 2;
                stopprinting = 0;                 
                break;
            }
            case INSTR_IMMOV:
            {                
                cout << current_instr << " immov " << processor.CPU_Memory[ptr]
                        << " "    << processor.CPU_Memory[ptr + 1] 
                                  << endl;
                ptr += 2;
                stopprinting = 0;                 
                break;
            }
            case INSTR_RRMOV:
            {                
                cout << current_instr << " rrmov " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr]) 
                        << " "    << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr + 1])
                                  << endl;
                ptr += 2;
                stopprinting = 0;                 
                break;
            }
            case INSTR_MRMOV:
            {                
                cout << current_instr << " mrmov " << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr])
                        << " "    << processor.CPU_Memory[ptr + 1]
                                  << endl;
                ptr += 2;
                stopprinting = 0;                 
                break;
            }
            case INSTR_RMMOV:
            {                
                cout << current_instr << " rmmov " << processor.CPU_Memory[ptr]
                        << " "    << processor.CPU_RF.reg_to_string(processor.CPU_Memory[ptr + 1])
                                  << endl;
                ptr += 2;
                stopprinting = 0;
                break;
            }
            case INSTR_MMMOV:
            {                
                cout << current_instr << " mmmov " << processor.CPU_Memory[ptr]
                        << " "    << processor.CPU_Memory[ptr + 1] 
                                  << endl;
                ptr += 2;
                stopprinting = 0;                 
                break;
            }
            default:
            {
                break;
            }
        }        
    }
}
void func1(CPU &processor)
{
    processor.immov(0, INSTR_IRMOV  );    // int user0 = 1;
    processor.immov(1, USER0        );
    processor.immov(2, 1            );    
    
    //*/
    processor.immov(3, INSTR_IRMOV  );    // int user1 = 2;
    processor.immov(4, USER1        );
    processor.immov(5, 2            );

    //*/
    processor.immov(6, INSTR_CMP    );  //
    processor.immov(7, USER0        );  //
    processor.immov(8, USER1        );  //  if (user0 != user1)
    //*/                                //  {
    processor.immov(9, INSTR_JE     );  //      user2 = 5;
    processor.immov(10, 16          );  //  }
    //*/
    processor.immov(11, INSTR_IRMOV );  //  else
    processor.immov(12, USER2       );  //  {
    processor.immov(13, 5           );  //      user2 = 4;
    //*/
    processor.immov(14, INSTR_JMP   );  //  }
    processor.immov(15, 19          );  //  halt;
    //*/
    processor.immov(16, INSTR_IRMOV );  //
    processor.immov(17, USER2       );  //
    processor.immov(18, 4           );  //
    processor.immov(19, INSTR_HALT  );  //
}
void add2(CPU &processor)
{
    // Hard-coding instructions into memory
    //
    // The following program is basically:
    //
    // int user0 = 0;
    // int user1 = 2;
    // user0 = user0 + user1;
    //
    processor.irmov(USER0, 0);
    processor.irmov(USER1, 2);
    
    // processor.irmov(USER0, 1);
    processor.immov(0, INSTR_IRMOV);
    processor.immov(1, USER0);
    processor.immov(2, 1);


    // processor.irmov(USER1, 2);
    processor.immov(3, INSTR_IRMOV);
    processor.immov(4, USER1);
    processor.immov(5, 2);

    // processor.add(USER0, USER1);
    processor.immov(6, INSTR_ADD);
    processor.immov(7, USER0);
    processor.immov(8, USER1);

    // processor.inc(USER0);
    processor.immov(9, INSTR_INC);
    processor.immov(10, USER0);
}
void multipleadd(CPU &processor)
{
    // Generating a sequence of 5 add instructions
    int last_spot = 0;
    for (int i = 0; i < 5; i++)
    {
        processor.immov(i * 3, INSTR_ADD);
        processor.immov(i * 3 + 1, USER0);
        processor.immov(i * 3 + 2, USER1);
        last_spot = (i + 1) * 3;
    }
    cout << last_spot << endl;
  processor.immov(last_spot, INSTR_HALT);  
}