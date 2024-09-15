#include <iostream>
#include "Memory.h"
#include "Processor.h"

using namespace std;

int main()
{
    Memory *mmu = new Memory("rom/bios.gb");
    Processor cpu(mmu);
    char c = cpu.read(0);
    cout << hex << uppercase << "0x" << unsigned(c) << endl;
    cpu.map(c);
    cpu.dump();
    return 0;
}
