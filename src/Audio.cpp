#include <fstream>
#include <string>
#include <stdint.h>
#include "Audio.h"
#include "Logger.h"

using namespace std;

Audio::Audio() {}

uint8_t Audio::readByte(uint16_t address)
{
    logger.logWord(__PRETTY_FUNCTION__, "Address", address);
    if (address <= 0xFF14)
        return square1[address & 0xF];
    switch (address & 0xFF)
    {
    case 0x26:
        return stat;
    default:
        logger.warn(__PRETTY_FUNCTION__, "Read Undefined Memory");
    }
}

void Audio::writeByte(uint16_t address, uint8_t byteValue)
{
    logger.logWord(__PRETTY_FUNCTION__, "Address", address);
    if (address <= 0xFF14)
    {
        square1[address & 0xF] = byteValue;
        return;
    }
    switch (address & 0xFF)
    {
    case 0x26:
        stat = byteValue;
        break;
    default:
        logger.error(__PRETTY_FUNCTION__, "Write Undefined Memory");
        exit(-2);
    }
}
