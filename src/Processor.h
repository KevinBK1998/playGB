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
    uint8_t read(uint16_t address);
};
