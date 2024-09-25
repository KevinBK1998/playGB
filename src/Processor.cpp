#include <iostream>
#include "Processor.h"
#include "Logger.h"

using namespace std;

Processor::Processor() : mmu(new Memory()), pc(0), sp(0) {}
Processor::Processor(Memory *mmu) : mmu(mmu), pc(0), sp(0) {}

uint8_t Processor::getA() { return a; }
uint8_t Processor::getC() { return c; }
uint8_t Processor::getF() { return f; }
uint16_t Processor::getHL() { return (h << 8) + l; }
uint16_t Processor::getPC() { return pc; }
uint16_t Processor::getSP() { return sp; }

void Processor::setHL(uint16_t wordValue)
{
    h = wordValue >> 8;
    l = wordValue;
}

void Processor::setPC(uint16_t wordValue) { pc = wordValue; }

void Processor::step()
{
    uint8_t opcode = mmu->readByte(pc);
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc++);
    logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
    logger.logByte(__PRETTY_FUNCTION__, "OpCode", opcode);
    map(opcode);
}

void Processor::dump()
{
    mmu->dump();
    ostringstream messageStream;
    messageStream << "CPU Registers" << hex << showbase << endl;
    messageStream << "\tA = " << unsigned(a) << ", C = " << unsigned(c) << ", F = " << unsigned(f) << endl;
    messageStream << "\tFLAGS: " << ((f & 0x80) != 0 ? "z" : "-") << ((f & 0x40) != 0 ? "n" : "-") << ((f & 0x20) != 0 ? "h" : "-") << ((f & 0x10) != 0 ? "c" : "-") << endl;
    messageStream << "\tPC = " << pc << ", SP = " << sp << ", HL = " << getHL();
    logger.debug(__PRETTY_FUNCTION__, messageStream.str());
}

void Processor::map(uint8_t opcode)
{
    switch (opcode)
    {
    case 0xE:
        ld_c_n();
        break;
    case 0x20:
        jr_nz_n();
        break;
    case 0x21:
        ld_hl_nn();
        break;
    case 0x31:
        ld_sp_nn();
        break;
    case 0x32:
        ldd_HL_a();
        break;
    case 0x3E:
        ld_a_n();
        break;
    case 0xAF:
        xor_a();
        break;
    case 0xCB:
        prefixMap(mmu->readByte(pc++));
        break;
    case 0xE2:
        ld_HC_a();
        break;

    default:
        dump();
        logger.error(__PRETTY_FUNCTION__, "UNKNOWN OPCODE");
        logger.logByte(__PRETTY_FUNCTION__, "OpCode", opcode);
        exit(-1);
    }
}

// 0x0E
void Processor::ld_c_n()
{
    logger.info(__PRETTY_FUNCTION__, "LD C, N");
    c = mmu->readByte(pc++);
    logger.logByte(__PRETTY_FUNCTION__, "C", c);
}

// 0x2*
void Processor::jr_nz_n()
{
    ostringstream messageStream;
    int8_t n = mmu->readByte(pc++);
    messageStream << "JR NZ, " << (int)n;
    logger.info(__PRETTY_FUNCTION__, messageStream.str());
    if (!(f & 0x80))
        pc += n;
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
}

void Processor::ld_hl_nn()
{
    logger.info(__PRETTY_FUNCTION__, "LDHL");
    setHL(mmu->readWord(pc));
    pc += 2;
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logWord(__PRETTY_FUNCTION__, "HL", getHL());
}

// 0x31
void Processor::ld_sp_nn()
{
    logger.info(__PRETTY_FUNCTION__, "LDSP");
    sp = mmu->readWord(pc);
    pc += 2;
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
}

void Processor::ldd_HL_a()
{
    logger.info(__PRETTY_FUNCTION__, "LDD[HL], A");
    mmu->writeByte(getHL(), a);
    setHL(getHL() - 1);
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logWord(__PRETTY_FUNCTION__, "HL", getHL());
}

void Processor::ld_a_n()
{
    logger.info(__PRETTY_FUNCTION__, "LD A, N");
    a = mmu->readByte(pc++);
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
}

// 0xAF
void Processor::xor_a()
{
    logger.info(__PRETTY_FUNCTION__, "XOR A");
    a = 0;
    f = 0x80;
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
    logger.logByte(__PRETTY_FUNCTION__, "F", f);
}

// 0xE2
void Processor::ld_HC_a()
{
    logger.info(__PRETTY_FUNCTION__, "LD [HC], A");
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
    logger.logByte(__PRETTY_FUNCTION__, "C", f);
    mmu->writeByte(0xFF00 + c, a);
}

void Processor::prefixMap(uint8_t opcode)
{
    switch (opcode)
    {
    case 0x7C:
        bit_h(7);
        break;

    default:
        dump();
        logger.error(__PRETTY_FUNCTION__, "UNKNOWN PREFIX OPCODE");
        exit(-1);
    }
}

// 0xCB 0x7C
void Processor::bit_h(int n)
{
    ostringstream messageStream;
    messageStream << "BIT " << n << ", H";
    logger.info(__PRETTY_FUNCTION__, messageStream.str());
    f &= 0x10;
    f |= 0x20;
    if (!((h >> n) & 1))
        f |= 0x80;
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logByte(__PRETTY_FUNCTION__, "H", h);
    logger.logByte(__PRETTY_FUNCTION__, "F", f);
}
