#include <fstream>
#include <string>
#include <stdint.h>
#include "Graphics.h"
#include "Logger.h"

using namespace std;

Graphics::Graphics() {}

uint8_t Graphics::readByte(uint16_t address)
{
    logger.logWord(__PRETTY_FUNCTION__, "Address", address);
    switch ((address & 0xF000) >> 12)
    {
    case 8:
    case 9:
        return vram[address & 0x1FFF];
    case 0xF:
        if (address == 0xFF47)
            return backgroundPalette;
        else
            logger.warn(__PRETTY_FUNCTION__, "Read Undefined 0xFFxx Memory");
    default:
        logger.warn(__PRETTY_FUNCTION__, "Read Undefined Memory");
    }
    return 0;
}

void Graphics::writeByte(uint16_t address, uint8_t byteValue)
{
    logger.logWord(__PRETTY_FUNCTION__, "Address", address);
    switch ((address & 0xF000) >> 12)
    {
    case 8:
    case 9:
        vram[address & 0x1FFF] = byteValue;
        break;
    case 0xF:
        if (address == 0xFF47)
            backgroundPalette = byteValue;
        else
        {
            logger.error(__PRETTY_FUNCTION__, "Write Undefined 0xFFxx Memory");
            exit(-2);
        }
        break;
    default:
        logger.error(__PRETTY_FUNCTION__, "Write Undefined Memory");
        exit(-2);
    }
}
