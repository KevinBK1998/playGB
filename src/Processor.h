#include <string>
#include "Memory.h"

class Processor
{
    Memory *mmu;
    uint8_t a, b, c, d, e, f;
    int m;
    uint8_t h, l;
    uint16_t pc, sp;
    void dump();
    void load(std::string regName, uint8_t *registerPtr, uint8_t byteValue);
    void loadImmediate(std::string regName, uint8_t *registerPtr);
    void push(std::string regName, uint16_t wordValue);
    void pop(std::string regName);
    // 0x05
    void dec_b();
    void inc_c();
    // 0x11
    void ld_de_nn();
    void rl_a();
    void ld_a_DE();
    // 0x2*
    void jr_nz_n();
    void ld_hl_nn();
    void ldi_HL_a();
    void inc_hl();
    // 0x31
    void ld_sp_nn();
    void ldd_HL_a();
    // 0x4F
    void ld_c_a();
    // 0x77
    void ld_HL_a();
    // 0xAF
    void xor_a();
    // 0xC5
    void pop_bc();
    void call_nn();
    // 0xE*
    void ld_HN_a();
    void ld_HC_a();

    // Prefix Opcodes
    //  0x11
    void rl_c();
    //  0x7C
    void bit_h(int n);

public:
    Processor();
    Processor(Memory *mmu);
    uint8_t getA();
    void setA(uint8_t byteValue);
    uint8_t getB();
    uint8_t getC();
    void setC(uint8_t byteValue);
    uint16_t getBC();
    void setBC(uint16_t wordValue);
    uint8_t getD();
    uint8_t getE();
    uint16_t getDE();
    void setDE(uint16_t wordValue);
    uint8_t getF();
    void setF(uint8_t byteValue);
    uint16_t getHL();
    void setHL(uint16_t wordValue);
    uint16_t getPC();
    void setPC(uint16_t wordValue);
    uint16_t getSP();
    void setSP(uint16_t wordValue);
    void step();
    void map(uint8_t opcode);
    void prefixMap(uint8_t opcode);
    int getMachineCycles();
};
