#include <iostream>
#include "Memory.h"
#include "Processor.h"
#include "Logger.h"

using namespace std;

int main()
{
    logger.info(__PRETTY_FUNCTION__, "START");
    Memory *mmu = new Memory("rom/bios.gb");
    Processor cpu(mmu);
    while (cpu.getPC() < 10)
    {
        cpu.step();
    }
    cpu.dump();
    free(mmu);
    return 0;
}
