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

Memory::Memory(std::string filename) : gpu(new Graphics()), apu(new Audio())
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

void Memory::dump()
{
    logger.setLogLevel(ERROR);
    ofstream dumpFile("dump.bin", ios::binary);
    if (!dumpFile.is_open())
    {
        logger.error(__PRETTY_FUNCTION__, "Error opening dump file");
    }
    int i = 0;
    while (i < 0x10000)
    {
        dumpFile << readByte(i++);
    }
}

uint8_t Memory::readByte(uint16_t address)
{
    logger.logWord(__PRETTY_FUNCTION__, "Address", address);
    switch ((address & 0xF000) >> 12)
    {
    case 0:
        return rom[address];
    case 8:
    case 9:
        return gpu->readByte(address);
    case 0xF:
        if ((address & 0xF00) == 0xF00)
        {
            switch (address & 0xF0)
            {
            case 0x10:
            case 0x20:
                return apu->readByte(address);

            default:
                logger.warn(__PRETTY_FUNCTION__, "Read Undefined 0xFFxx Memory");
                return 0;
            }
        }
    default:
        logger.warn(__PRETTY_FUNCTION__, "Read Undefined Memory");
        return 0;
    }
}

void Memory::writeByte(uint16_t address, uint8_t byteValue)
{
    logger.logWord(__PRETTY_FUNCTION__, "Address", address);
    switch (address & 0xF000)
    {
    case 0:
        rom[address] = byteValue;
        break;
    case 0x8000:
    case 0x9000:
        gpu->writeByte(address, byteValue);
        break;
    case 0xF000:
        if ((address & 0xF00) == 0xF00)
        {
            switch (address & 0xF0)
            {
            case 0x10:
            case 0x20:
                apu->writeByte(address, byteValue);
                break;

            default:
                logger.error(__PRETTY_FUNCTION__, "Write Undefined 0xFFxx Memory");
                exit(-2);
            }
        }
        break;
    default:
        logger.error(__PRETTY_FUNCTION__, "Write Undefined Memory");
        exit(-2);
    }
}

uint16_t Memory::readWord(uint16_t address)
{
    return (readByte(address + 1) << 8) + readByte(address);
}
