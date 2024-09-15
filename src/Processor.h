#include "Memory.h"
class Processor
{
    Memory *mmu;
    uint16_t pc;
    uint16_t sp;

public:
    Processor();
    Processor(Memory *mmu);
    uint16_t getPC();
    uint16_t getSP();
    void map(uint8_t opcode);
    void dump();
    uint8_t read(uint16_t address);
};
