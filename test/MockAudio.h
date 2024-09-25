#include <gmock/gmock.h>
#include "../src/Audio.h"

class MockAudio : public Audio
{
public:
    MockAudio() {}
    MOCK_METHOD(uint8_t, readByte, (uint16_t address), (override));
    MOCK_METHOD(void, writeByte, (uint16_t address, uint8_t byteValue), (override));
};