#include <gmock/gmock.h>
#include "../src/Memory.h"

class MockMemory : public Memory
{
public:
  MockMemory() {}
  MOCK_METHOD(uint8_t, readByte, (uint16_t address), (override));
  MOCK_METHOD(uint16_t, readWord, (uint16_t address), (override));
};