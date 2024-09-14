#include <fstream>
#include <string>
#include "memory.h"

using namespace std;

int load(string filename)
{
    char c;
    ifstream fin(filename);
    if (fin.get(c))
        return c;
    return -1;
}
