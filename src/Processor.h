#include "Memory.h"

class Processor
{
    Memory *mmu;
    uint8_t a, c, f;
    uint8_t h, l;
    uint16_t pc, sp;
    void dump();
    // 0x0*
    void ld_c_n();
    // 0x2*
    void jr_nz_n();
    void ld_hl_nn();
    // 0x31
    void ld_sp_nn();
    void ldd_HL_a();
    void ld_a_n();
    // 0xAF
    void xor_a();
    // 0xE2
    void ld_HC_a();

    // Prefix Opcodes
    //  0x7C
    void bit_h(int n);

public:
    Processor();
    Processor(Memory *mmu);
    uint8_t getA();
    uint8_t getC();
    uint8_t getF();
    uint16_t getHL();
    void setHL(uint16_t wordValue);
    uint16_t getPC();
    void setPC(uint16_t wordValue);
    uint16_t getSP();
    void step();
    void map(uint8_t opcode);
    void prefixMap(uint8_t opcode);
};
