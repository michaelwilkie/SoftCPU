#ifndef _MEMORY_H
#define _MEMORY_H
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <string>

#include <limits>

#define MAX_MEMORY 1024 * 1024 * 8 // 8 megabytes

using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::string;

class Memory
{
public:
    uint64_t memsize = MAX_MEMORY / sizeof(uint64_t);
    uint64_t *mem_start_addr = NULL; // gave start address a name, strictly for semantics
    uint64_t *mem = NULL;            // will be a duplicate of mem_start_addr, more semantics

    Memory()
    {
        initialize_memory();

        uint64_t fd = open("mainmemory", O_RDWR | O_CREAT, 0600);

        void *addr = mmap(NULL, memsize * sizeof(uint64_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        mem_start_addr = (uint64_t*) addr;

        cout << "Memory address: " << mem_start_addr << endl;
        cout << "Max Memory: " << memsize << endl;
        
        if (addr == (void*) -1 ) 
        {
            cout << "Failed to open memory file. Exiting..." << endl;
            exit(0);
        }

        mem = mem_start_addr;
    }
    void initialize_memory()
    {
        ofstream memfile("mainmemory");
        if (!memfile)
        {
            cout << "Failed to open memory file. Exiting..." << endl;
            exit(0);
        }
        char *garbo = (char*)calloc(MAX_MEMORY, sizeof(uint64_t));
        memfile.write(garbo, MAX_MEMORY * sizeof(uint64_t));
        free(garbo);
    }
    ~Memory()
    {
        munmap(mem_start_addr, memsize);
    }
    uint64_t operator[](uint64_t addr) 
    { 
        uint64_t val = 0; 
        uint64_t* ptr = &val;
        memcpy(ptr, mem_start_addr + addr, 8);
        return *ptr; 
    }
    uint32_t operator[](uint32_t addr) 
    { 
        uint32_t val = 0; 
        uint32_t* ptr = &val; 
        memcpy(ptr, (uint32_t*)mem_start_addr + addr, 4); 
        return *ptr; 
    }
    uint16_t operator[](uint16_t addr) 
    { 
        uint16_t val = 0; 
        uint16_t* ptr = &val; 
        memcpy(ptr, (uint16_t*)mem_start_addr + addr, 2); 
        return *ptr; 
    }
    uint8_t  operator[](uint8_t  addr) 
    { 
        uint8_t  val = 0; 
        uint8_t * ptr = &val; 
        memcpy(ptr, (uint8_t*)mem_start_addr + addr, 1); 
        return *ptr; 
    }

    void write(uint64_t addr, uint64_t val) { memcpy((uint64_t*)mem_start_addr + addr, &val, 8) ; }
    void write(uint32_t addr, uint32_t val) { memcpy((uint32_t*)mem_start_addr + addr, &val, 4) ; }
    void write(uint16_t addr, uint16_t val) { memcpy((uint16_t*)mem_start_addr + addr, &val, 2) ; }
    void write(uint16_t addr,     char val) { memcpy((uint16_t*)mem_start_addr + addr, &val, 2) ; }
    void write(uint8_t  addr, uint8_t  val) { memcpy((uint8_t *)mem_start_addr + addr, &val, 1) ; }
};
#endif