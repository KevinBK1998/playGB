#include <fstream>
#include <string>
#include <stdint.h>
#include "Memory.h"
#include "Logger.h"

using namespace std;

Memory::Memory()
{
    for (int i = 0; i < 256; i++)
    {
        rom[i] = 0;
    }
}

Memory::Memory(Graphics *gpu) : gpu(gpu) {}
Memory::Memory(Audio *apu) : apu(apu) {}

Memory::Memory(std::string filename)
{
    ifstream fin(filename);
    char c;
    int i = 0;
    while (i < 256 && fin)
    {
        fin.get(c);
        rom[i++] = c;
    }
    fin.close();
}

uint8_t Memory::readByte(uint16_t address)
{
    switch ((address & 0xF000) >> 12)
    {
    case 0:
        return rom[address];
    case 8:
    case 9:
        return gpu->readByte(address);
    case 0xF:
        if (address == 0xFF26)
        {
            return apu->readByte(address);
        }
    default:
        logger.warn(__PRETTY_FUNCTION__, "Read Undefined Memory");
        logger.logWord(__PRETTY_FUNCTION__, "Address", address);
        return 0;
    }
}

void Memory::writeByte(uint16_t address, uint8_t byteValue)
{
    switch ((address & 0xF000) >> 12)
    {
    case 0:
        rom[address] = byteValue;
        break;
    case 8:
    case 9:
        gpu->writeByte(address, byteValue);
        break;
    case 0xF:
        if (address == 0xFF26)
        {
            apu->writeByte(address, byteValue);
            break;
        }
    default:
        logger.error(__PRETTY_FUNCTION__, "Write Undefined Memory");
        logger.logWord(__PRETTY_FUNCTION__, "Address", address);
        exit(-2);
    }
}

uint16_t Memory::readWord(uint16_t address)
{
    return (readByte(address + 1) << 8) + readByte(address);
}
