#include <gmock/gmock.h>
#include "../src/Graphics.h"

class MockGraphics : public Graphics
{
public:
    MockGraphics() {}
    MOCK_METHOD(uint8_t, readByte, (uint16_t address), (override));
    MOCK_METHOD(void, writeByte, (uint16_t address, uint8_t byteValue), (override));
};