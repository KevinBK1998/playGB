#include <iostream>
#include "Processor.h"
#include "Logger.h"

using namespace std;

Processor::Processor() : mmu(new Memory()), pc(0), sp(0), m(0) {}
Processor::Processor(Memory *mmu) : mmu(mmu), pc(0), sp(0), m(0) {}

uint8_t Processor::getA() { return a; }
uint8_t Processor::getB() { return b; }
uint8_t Processor::getC() { return c; }
uint8_t Processor::getD() { return d; }
uint8_t Processor::getE() { return e; }
uint8_t Processor::getF() { return f; }
uint16_t Processor::getAF() { return (a << 8) + f; }
uint16_t Processor::getBC() { return (b << 8) + c; }
uint16_t Processor::getDE() { return (d << 8) + e; }
uint16_t Processor::getHL() { return (h << 8) + l; }
uint16_t Processor::getPC() { return pc; }
uint16_t Processor::getSP() { return sp; }

int Processor::getMachineCycles() { return m; }

void Processor::setA(uint8_t byteValue) { a = byteValue; }
void Processor::setC(uint8_t byteValue) { c = byteValue; }
void Processor::setE(uint8_t byteValue) { e = byteValue; }
void Processor::setF(uint8_t byteValue) { f = byteValue; }

void Processor::setBC(uint16_t wordValue)
{
    b = wordValue >> 8;
    c = wordValue;
}

void Processor::setDE(uint16_t wordValue)
{
    d = wordValue >> 8;
    e = wordValue;
}

void Processor::setHL(uint16_t wordValue)
{
    h = wordValue >> 8;
    l = wordValue;
}

void Processor::setPC(uint16_t wordValue) { pc = wordValue; }
void Processor::setSP(uint16_t wordValue) { sp = wordValue; }

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
    messageStream << "\tA = " << unsigned(a) << ", F = " << unsigned(f) << ", AF = " << getAF() << endl;
    messageStream << "\tB = " << unsigned(b) << ", C = " << unsigned(c) << ", BC = " << getBC() << endl;
    messageStream << "\tD = " << unsigned(d) << ", E = " << unsigned(e) << ", DE = " << getDE() << endl;
    messageStream << "\tH = " << unsigned(h) << ", L = " << unsigned(l) << ", HL = " << getHL() << endl;
    messageStream << "\tFLAGS: " << ((f & 0x80) != 0 ? "z" : "-") << ((f & 0x40) != 0 ? "n" : "-") << ((f & 0x20) != 0 ? "h" : "-") << ((f & 0x10) != 0 ? "c" : "-") << endl;
    messageStream << "\tPC = " << pc << ", SP = " << sp << ", Machine Uptime = " << dec << m;
    logger.setLogLevel(DEBUG);
    logger.debug(__PRETTY_FUNCTION__, messageStream.str());
}

void Processor::map(uint8_t opcode)
{
    switch (opcode)
    {
    case 0:
        logger.info(__PRETTY_FUNCTION__, "NOP");
        logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
        break;
    case 0x5:
        dec_b();
        break;
    case 0x6:
        loadImmediate("B", &b);
        break;
    case 0xC:
        inc_c();
        break;
    case 0xE:
        loadImmediate("C", &c);
        break;
    case 0x11:
        ld_de_nn();
        break;
    case 0x13:
        inc_de();
        break;
    case 0x17:
        rl_a();
        break;
    case 0x1A:
        ld_a_DE();
        break;
    case 0x20:
        jr_nz_n();
        break;
    case 0x21:
        ld_hl_nn();
        break;
    case 0x22:
        ldi_HL_a();
        break;
    case 0x23:
        inc_hl();
        break;
    case 0x28:
        jumpRelativeZero();
        break;
    case 0x31:
        ld_sp_nn();
        break;
    case 0x32:
        ldd_HL_a();
        break;
    case 0x3D:
        decA();
        break;
    case 0x3E:
        loadImmediate("A", &a);
        break;
    case 0x4F:
        logger.info(__PRETTY_FUNCTION__, "LD C, A");
        load("C", &c, a);
        break;
    case 0x77:
        ld_HL_a();
        break;
    case 0x7B:
        logger.info(__PRETTY_FUNCTION__, "LD A, E");
        load("A", &a, e);
        break;
    case 0xAF:
        xor_a();
        break;
    case 0xC1:
        pop("BC");
        break;
    case 0xC5:
        push("BC", getBC());
        break;
    case 0xC9:
        ret();
        break;
    case 0xCB:
        prefixMap(mmu->readByte(pc++));
        break;
    case 0xCD:
        call_nn();
        break;
    case 0xE0:
        ld_HN_a();
        break;
    case 0xE2:
        ld_HC_a();
        break;
    case 0xEA:
        ldNN_a();
        break;
    case 0xFE:
        cp_n();
        break;

    default:
        dump();
        logger.error(__PRETTY_FUNCTION__, "UNKNOWN OPCODE");
        logger.logByte(__PRETTY_FUNCTION__, "OpCode", opcode);
        exit(-1);
    }
}

void Processor::jumpRelativeZero()
{
    logger.info(__PRETTY_FUNCTION__, "JR Z, N");
    int8_t n = mmu->readByte(pc++);
    m += 2;
    if (f & 0x80)
    {
        pc += n;
        m++;
    }
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logByte(__PRETTY_FUNCTION__, "F", f);
}

void Processor::decA()
{
    logger.info(__PRETTY_FUNCTION__, "DEC A");
    uint8_t result = a - 1;
    f &= 0x10;
    if (!result)
        f |= 0x80;
    f |= 0x40;
    if ((a & 0xF) - 1 < 0)
        f |= 0x20;
    a = result;
    m++;
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
    logger.logByte(__PRETTY_FUNCTION__, "F", f);
}

void Processor::load(string regName, uint8_t *registerPtr, uint8_t byteValue)
{
    *registerPtr = byteValue;
    m++;
    logger.logByte(__PRETTY_FUNCTION__, regName, *registerPtr);
}

void Processor::loadImmediate(string regName, uint8_t *registerPtr)
{
    uint8_t n = mmu->readByte(pc++);
    m++;
    ostringstream messageStream;
    messageStream << "LD " << regName << ", " << hex << showbase << unsigned(n);
    logger.debug(__PRETTY_FUNCTION__, messageStream.str());
    load(regName, registerPtr, n);
}

void Processor::pop(string regName)
{
    ostringstream messageStream;
    messageStream << "POP " << regName;
    logger.debug(__PRETTY_FUNCTION__, messageStream.str());
    uint16_t wordValue = mmu->readWord(sp);
    sp += 2;
    if (!regName.compare("BC"))
    {
        setBC(wordValue);
    }
    logger.logWord(__PRETTY_FUNCTION__, regName, wordValue);
    logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
    m += 3;
}

void Processor::push(string regName, uint16_t wordValue)
{
    ostringstream messageStream;
    messageStream << "PUSH " << regName;
    logger.debug(__PRETTY_FUNCTION__, messageStream.str());
    sp -= 2;
    mmu->writeWord(sp, wordValue);
    logger.logWord(__PRETTY_FUNCTION__, regName, wordValue);
    logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
    m += 3;
}

// 0x05
void Processor::dec_b()
{
    logger.info(__PRETTY_FUNCTION__, "DEC B");
    uint8_t result = b - 1;
    f &= 0x10;
    if (!result)
        f |= 0x80;
    if ((b & 0xF) + 1 > 0xF)
        f |= 0x20;
    b = result;
    logger.logByte(__PRETTY_FUNCTION__, "B", b);
    logger.logByte(__PRETTY_FUNCTION__, "F", f);
}

void Processor::inc_c()
{
    logger.info(__PRETTY_FUNCTION__, "INC C");
    uint8_t result = c + 1;
    f &= 0x10;
    if (!result)
        f |= 0x80;
    if ((c & 0xF) + 1 > 0xF)
        f |= 0x20;
    c = result;
    logger.logByte(__PRETTY_FUNCTION__, "C", c);
    logger.logByte(__PRETTY_FUNCTION__, "F", f);
}

// 0x11
void Processor::ld_de_nn()
{
    logger.info(__PRETTY_FUNCTION__, "LD DE, NN");
    uint16_t nn = mmu->readWord(pc);
    pc += 2;
    setDE(nn);
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logWord(__PRETTY_FUNCTION__, "DE", getDE());
}

void Processor::inc_de()
{
    logger.info(__PRETTY_FUNCTION__, "INC DE");
    setDE(getDE() + 1);
    m++;
    logger.logWord(__PRETTY_FUNCTION__, "DE", getDE());
}

void Processor::ld_a_DE()
{
    logger.info(__PRETTY_FUNCTION__, "LD A, [DE]");
    a = mmu->readByte(getDE());
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
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
    logger.info(__PRETTY_FUNCTION__, "LD HL, NN");
    setHL(mmu->readWord(pc));
    pc += 2;
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logWord(__PRETTY_FUNCTION__, "HL", getHL());
}

void Processor::ldi_HL_a()
{
    logger.info(__PRETTY_FUNCTION__, "LDI[HL], A");
    mmu->writeByte(getHL(), a);
    setHL(getHL() + 1);
    m += 2;
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logWord(__PRETTY_FUNCTION__, "HL", getHL());
}

void Processor::inc_hl()
{
    logger.info(__PRETTY_FUNCTION__, "INC HL");
    setHL(getHL() + 1);
    m++;
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

// 0x77
void Processor::ld_HL_a()
{
    logger.info(__PRETTY_FUNCTION__, "LD [HL], A");
    mmu->writeByte(getHL(), a);
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logWord(__PRETTY_FUNCTION__, "HL", getHL());
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

// 0xC5
void Processor::call_nn()
{
    logger.info(__PRETTY_FUNCTION__, "CALL NN");
    sp -= 2;
    uint16_t nn = mmu->readWord(pc);
    mmu->writeWord(sp, pc + 2);
    pc = nn;
    logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
}

void Processor::ret()
{
    logger.info(__PRETTY_FUNCTION__, "RET");
    uint16_t nn = mmu->readWord(sp);
    sp += 2;
    pc = nn;
    m += 3;
    logger.logWord(__PRETTY_FUNCTION__, "SP", sp);
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
}

// 0xE*
void Processor::ld_HN_a()
{
    logger.info(__PRETTY_FUNCTION__, "LD [HN], A");
    uint8_t n = mmu->readByte(pc++);
    logger.logByte(__PRETTY_FUNCTION__, "N", n);
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
    mmu->writeByte(0xFF00 + n, a);
}

void Processor::ld_HC_a()
{
    logger.info(__PRETTY_FUNCTION__, "LD [HC], A");
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
    logger.logByte(__PRETTY_FUNCTION__, "C", c);
    mmu->writeByte(0xFF00 + c, a);
}

void Processor::ldNN_a()
{
    logger.info(__PRETTY_FUNCTION__, "LD [NN], A");
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
    uint16_t nn = mmu->readWord(pc);
    logger.logWord(__PRETTY_FUNCTION__, "NN", nn);
    mmu->writeByte(nn, a);
    pc += 2;
    m += 4;
}

void Processor::cp_n()
{
    logger.info(__PRETTY_FUNCTION__, "CP N");
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
    uint8_t n = mmu->readByte(pc);
    logger.logByte(__PRETTY_FUNCTION__, "N", n);
    int res = a - n;
    f = 0x40;
    if (!(res & 0xFF))
        f |= 0x80;
    if ((a & 0xF) - (n & 0xF) < 0)
        f |= 0x20;
    if (res < 0)
        f |= 0x10;
    logger.logByte(__PRETTY_FUNCTION__, "F", f);
    pc++;
    m += 2;
}

void Processor::prefixMap(uint8_t opcode)
{
    switch (opcode)
    {
    case 0x11:
        rl_c();
        break;
    case 0x17:
        rl_a();
        m++;
        break;
    case 0x7C:
        bit_h(7);
        break;

    default:
        dump();
        logger.error(__PRETTY_FUNCTION__, "UNKNOWN PREFIX OPCODE");
        logger.logByte(__PRETTY_FUNCTION__, "OpCode", opcode);
        exit(-1);
    }
}

// 0xCB 0x11
void Processor::rl_c()
{
    logger.info(__PRETTY_FUNCTION__, "RL C");
    int carry = ((f & 0x10) != 0);
    f = 0;
    f |= ((c & 0x80) >> 3);
    c <<= 1;
    c += carry;
    if (!c)
        f |= 0x80;
    m += 2;
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logByte(__PRETTY_FUNCTION__, "C", c);
    logger.logByte(__PRETTY_FUNCTION__, "F", f);
}

void Processor::rl_a()
{
    logger.info(__PRETTY_FUNCTION__, "RL A");
    int carry = ((f & 0x10) != 0);
    f = 0;
    f |= ((a & 0x80) >> 3);
    a <<= 1;
    a += carry;
    if (!a)
        f |= 0x80;
    m++;
    logger.logWord(__PRETTY_FUNCTION__, "PC", pc);
    logger.logByte(__PRETTY_FUNCTION__, "A", a);
    logger.logByte(__PRETTY_FUNCTION__, "F", f);
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
