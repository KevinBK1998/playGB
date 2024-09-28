#include <gtest/gtest.h>
#include "../src/Audio.h"

using ::testing::Range;

class AudioTest : public testing::TestWithParam<int>
{
};

TEST_P(AudioTest, writeSquare1)
{
    Audio apu;
    apu.writeByte(GetParam(), 0x80);
    ASSERT_EQ(apu.readByte(GetParam()), 0x80);
}

INSTANTIATE_TEST_SUITE_P(Param, AudioTest, Range(0xFF10, 0xFF15));

TEST(AudioTest, writeStat)
{
    Audio apu;
    apu.writeByte(0xff26, 0x80);
    ASSERT_EQ(apu.readByte(0xff26), 0x80);
}

TEST(AudioTest, writeEnable)
{
    Audio apu;
    apu.writeByte(0xff25, 0x80);
    ASSERT_EQ(apu.readByte(0xff25), 0x80);
}
