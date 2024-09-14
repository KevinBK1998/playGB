#include <iostream>
#include "Memory.h"

using namespace std;

int main()
{
    // char c;
    char c = load("rom/bios.gb");
    // ifstream fin("rom/bios.gb");
    // fin.get(c);
    cout << hex << uppercase << unsigned(c) << endl;
    return 0;
}
