#include <string>
#include <stdint.h>

#ifndef AUDIO_H
#define AUDIO_H

class Audio
{
    uint8_t square1[5];
    uint8_t stat;

public:
    Audio();
    virtual uint8_t readByte(uint16_t address);
    virtual void writeByte(uint16_t address, uint8_t byteValue);
};

#endif
