#include <iostream>
#include "Processor.h"
#include "Logger.h"

using namespace std;

Processor::Processor() : mmu(new Memory()), pc(0), sp(0) {}
Processor::Processor(Memory *mmu) : mmu(mmu), pc(0), sp(0) {}

uint8_t Processor::getA() { return a; }
uint8_t Processor::getF() { return f; }
uint16_t Processor::getPC() { return pc; }
uint16_t Processor::getSP() { return sp; }

void Processor::step()
{
    uint8_t opcode = mmu->readByte(pc);
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc++);
    logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
    logger.logByte(__PRETTY_FUNCTION__, "OpCode", opcode);
    map(opcode);
}

void Processor::map(uint8_t opcode)
{
    if (opcode == 0x31)
    {
        logger.info(__PRETTY_FUNCTION__, "LDSP");
        sp = mmu->readWord(pc);
        pc += 2;
        logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
        logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
    }
    else if (opcode == 0xAF)
    {
        logger.info(__PRETTY_FUNCTION__, "XOR A");
        a = 0;
        f = 0x80;
        logger.logByte(__PRETTY_FUNCTION__, "A", a);
        logger.logByte(__PRETTY_FUNCTION__, "F", f);
    }
    else
    {
        logger.error(__PRETTY_FUNCTION__, "UNKNOWN");
        exit(-1);
    }
}

void Processor::dump()
{
    ostringstream messageStream;
    messageStream << "CPU Registers" << hex << showbase << endl;
    messageStream << "\tA = " << unsigned(a) << ", F = " << unsigned(f) << endl;
    messageStream << "\tPC = " << pc << ", SP = " << sp;
    logger.debug(__PRETTY_FUNCTION__, messageStream.str());
}

uint8_t Processor::read(uint16_t address)
{
    return mmu->readByte(address);
}
