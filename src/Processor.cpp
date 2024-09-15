#include <iostream>
#include "Processor.h"

using namespace std;

Processor::Processor() : mmu(new Memory()), pc(0), sp(0) {}

Processor::Processor(Memory *mmu) : mmu(mmu), pc(0), sp(0) {}

uint16_t Processor::getPC()
{
    return pc;
}

uint16_t Processor::getSP()
{
    return sp;
}

uint8_t Processor::read(uint16_t address)
{
    return mmu->readByte(address);
}
