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
    char c = cpu.read(0);
    logger.logByte(__PRETTY_FUNCTION__, "c", c);
    cpu.map(c);
    c = cpu.read(3);
    logger.logByte(__PRETTY_FUNCTION__, "c", c);
    cpu.map(c);
    cpu.dump();
    free(mmu);
    return 0;
}
