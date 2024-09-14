#include <string>
#include <stdint.h>

int load(std::string filename);

class Memory
{
    uint8_t rom[256];

public:
    Memory(std::string filename);
    uint8_t read(uint16_t address);
};
