#include <iostream>
#include "Processor.h"
#include "Logger.h"

using namespace std;

Processor::Processor() : mmu(new Memory()), pc(0), sp(0) {}

Processor::Processor(Memory *mmu) : mmu(mmu), pc(0), sp(0) {}

uint16_t Processor::getPC() { return pc; }

uint16_t Processor::getSP() { return sp; }

void Processor::map(uint8_t opcode)
{
    logger.logByte(__PRETTY_FUNCTION__, "OpCode", opcode);
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc++);
    logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
    if (opcode == 0x31)
    {
        logger.log(INFO, __PRETTY_FUNCTION__, "LDSP");
        sp = mmu->readWord(pc);
        pc += 2;
    }
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
}

void Processor::dump()
{
    ostringstream messageStream;
    messageStream << hex << showbase << "PC = " << pc << ", SP = " << sp;
    logger.debug(__PRETTY_FUNCTION__, messageStream.str());
}

uint8_t Processor::read(uint16_t address)
{
    return mmu->readByte(address);
}
