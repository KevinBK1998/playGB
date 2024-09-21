#include <iostream>
#include "Memory.h"
#include "Processor.h"
#include "Logger.h"

using namespace std;

int main()
{
    Memory *mmu = new Memory("rom/bios.gb");
    Processor cpu(mmu);
    char c = cpu.read(0);
    logger.logByte(__PRETTY_FUNCTION__, "c", c);
    cpu.map(c);
    logger.log(INFO, __PRETTY_FUNCTION__, "TEST");
    cpu.dump();
    free(mmu);
    return 0;
}
