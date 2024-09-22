#include <iostream>
#include "Memory.h"
#include "Processor.h"
#include "Logger.h"

using namespace std;

int main()
{
    uint16_t BREAK_POINT = 0xC;
    logger.info(__PRETTY_FUNCTION__, "START");
    Memory *mmu = new Memory("rom/bios.gb");
    Processor cpu(mmu);
    while (cpu.getPC() < 20)
    {
        if (cpu.getPC() == BREAK_POINT)
        {
            logger.setLogLevel(DEBUG);
            logger.info(__PRETTY_FUNCTION__, "START");
        }
        cpu.step();
    }
    cpu.dump();
    free(mmu);
    return 0;
}
