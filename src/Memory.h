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
    virtual uint8_t readByte(uint16_t address);
    virtual uint16_t readWord(uint16_t address);
};

#endif
