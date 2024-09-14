#include <iostream>
#include <fstream>
#include "myFunctions.h"
#include "Memory.h"

using namespace std;

int main()
{
    cout << factorial(6) << endl;
    // char c;
    char c = load("rom/bios.gb");
    // ifstream fin("rom/bios.gb");
    // fin.get(c);
    cout << hex << uppercase << unsigned(c) << endl;
    return 0;
}
