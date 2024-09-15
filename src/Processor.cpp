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

void Processor::map(uint8_t opcode)
{
    cout << hex << uppercase << "PC: 0x" << pc++ << endl;
    cout << hex << uppercase << "OPCODE: 0x" << unsigned(opcode) << endl;
    if (opcode == 0x31)
    {
        cout << "Operation: ldsp" << endl;
        sp = mmu->readWord(pc);
        pc += 2;
    }
}

void Processor::dump()
{
    cout << hex << uppercase << "pc:" << pc << "\tsp:" << sp << endl;
}

uint8_t Processor::read(uint16_t address)
{
    return mmu->readByte(address);
}
