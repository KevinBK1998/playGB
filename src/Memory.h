#include <string>
#include <stdint.h>

#ifndef MEM_h
#define MEM_h

int load(std::string filename);

class Memory
{
    uint8_t rom[256];

public:
    Memory();
    Memory(std::string filename);
    virtual uint8_t read(uint16_t address);
};

#endif
