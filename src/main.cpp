#include <iostream>
#include "Memory.h"
#include "Processor.h"
#include "Logger.h"

using namespace std;

int main()
{
    uint16_t BREAK_POINT = 0x40;
    logger.info(__PRETTY_FUNCTION__, "START");
    Memory *mmu = new Memory("rom/bios.gb", "rom/ttt.gb");
    Processor cpu(mmu);

    while (cpu.getPC() < 0x50 || (cpu.getPC() >= 0x95 && cpu.getPC() < 0xB0))
    {
        if (cpu.getPC() == BREAK_POINT)
        {
            logger.setLogLevel(DEBUG);
            logger.info(__PRETTY_FUNCTION__, "BRKP");
        }
        cpu.step();
    }

    return 0;
}
