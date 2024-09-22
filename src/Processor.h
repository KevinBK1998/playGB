#include "Memory.h"

class Processor
{
    Memory *mmu;
    uint8_t a, f;
    uint8_t h, l;
    uint16_t pc, sp;
    // 0x21
    void ld_hl_nn();
    // 0x31
    void ld_sp_nn();
    void ldd_HL_a();
    // 0xAF
    void xor_a();

    // Prefix Opcodes
    //  0x7C
    void bit_h(int n);

public:
    Processor();
    Processor(Memory *mmu);
    uint8_t getA();
    uint8_t getF();
    uint16_t getHL();
    void setHL(uint16_t wordValue);
    uint16_t getPC();
    uint16_t getSP();
    void step();
    void map(uint8_t opcode);
    void prefixMap(uint8_t opcode);
    void dump();
};
