#include <fstream>
#include <string>
#include <stdint.h>
#include "Graphics.h"
#include "Logger.h"

using namespace std;

Graphics::Graphics() {}

uint8_t Graphics::readByte(uint16_t address)
{
    switch ((address & 0xF000) >> 12)
    {
    case 8:
    case 9:
        return vram[address & 0x1FFF];
    default:
        logger.warn(__PRETTY_FUNCTION__, "Read Undefined Memory");
        logger.logWord(__PRETTY_FUNCTION__, "Address", address);
    }
}

void Graphics::writeByte(uint16_t address, uint8_t byteValue)
{
    switch ((address & 0xF000) >> 12)
    {
    case 8:
    case 9:
        vram[address & 0x1FFF] = byteValue;
        break;
    default:
        logger.error(__PRETTY_FUNCTION__, "Write Undefined Memory");
        logger.logWord(__PRETTY_FUNCTION__, "Address", address);
        exit(-2);
    }
}
