#include <fstream>
#include <string>
#include <stdint.h>
#include "Memory.h"

using namespace std;

Memory::Memory()
{
    for (int i = 0; i < 256; i++)
    {
        rom[i] = 0;
    }
}

Memory::Memory(std::string filename)
{
    ifstream fin(filename);
    char c;
    int i = 0;
    while (fin)
    {
        fin.get(c);
        rom[i++] = c;
    }
}

uint8_t Memory::read(uint16_t address)
{
    return rom[address];
}
