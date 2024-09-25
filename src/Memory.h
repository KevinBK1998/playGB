#include <string>
#include <stdint.h>
#include "Graphics.h"
#include "Audio.h"

#ifndef MEM_H
#define MEM_H

class Memory
{
    uint8_t rom[256];
    Graphics *gpu;
    Audio *apu;

public:
    Memory();
    Memory(Graphics *gpu);
    Memory(Audio *apu);
    Memory(std::string filename);
    virtual uint8_t readByte(uint16_t address);
    virtual uint16_t readWord(uint16_t address);
    virtual void writeByte(uint16_t address, uint8_t byteValue);
};

#endif
