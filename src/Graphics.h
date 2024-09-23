#include <string>
#include <stdint.h>

#ifndef GRAPHICS_H
#define GRAPHICS_H

class Graphics
{
    uint8_t vram[8192];

public:
    Graphics();
    virtual uint8_t readByte(uint16_t address);
    virtual void writeByte(uint16_t address, uint8_t byteValue);
};

#endif
