#include <fstream>
#include <string>
#include <stdint.h>
#include "Audio.h"
#include "Logger.h"

using namespace std;

Audio::Audio() {}

uint8_t Audio::readByte(uint16_t address)
{
    switch (address & 0xFF)
    {
    case 0x26:
        return stat;
    default:
        logger.warn(__PRETTY_FUNCTION__, "Read Undefined Memory");
        logger.logWord(__PRETTY_FUNCTION__, "Address", address);
    }
}

void Audio::writeByte(uint16_t address, uint8_t byteValue)
{
    switch (address & 0xFF)
    {
    case 0x26:
        stat = byteValue;
        break;
    default:
        logger.error(__PRETTY_FUNCTION__, "Write Undefined Memory");
        logger.logWord(__PRETTY_FUNCTION__, "Address", address);
        exit(-2);
    }
}
