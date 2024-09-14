#include <iostream>
#include "Memory.h"

using namespace std;

int main()
{
    Memory mmu("rom/bios.gb");
    char c = mmu.read(0);
    cout << hex << uppercase << "0x" << unsigned(c) << endl;
    c = mmu.read(1);
    cout << hex << uppercase << "0x" << unsigned(c) << endl;
    return 0;
}
