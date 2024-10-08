#include <string>
#include <stdint.h>

#ifndef MEM_H
#define MEM_H

class Memory
{
    uint8_t rom[256];
    uint8_t vram[8192];

public:
    Memory();
    Memory(std::string filename);
    virtual uint8_t readByte(uint16_t address);
    virtual uint16_t readWord(uint16_t address);
    virtual void writeByte(uint16_t address, uint8_t byteValue);
};

#endif
