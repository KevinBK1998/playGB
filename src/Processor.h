#include "Memory.h"

class Processor
{
    Memory *mmu;
    uint8_t a, f;
    uint16_t pc, sp;

public:
    Processor();
    Processor(Memory *mmu);
    uint8_t getA();
    uint8_t getF();
    uint16_t getPC();
    uint16_t getSP();
    void step();
    void map(uint8_t opcode);
    void dump();
    uint8_t read(uint16_t address);
};
