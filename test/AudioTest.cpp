#include <gtest/gtest.h>
#include "../src/Audio.h"

TEST(AudioTest, writeStat)
{
    Audio apu;
    apu.writeByte(0xff26, 0x80);
    ASSERT_EQ(apu.readByte(0xff26), 0x80);
}

TEST(AudioTest, writeSquare1)
{
    Audio apu;
    apu.writeByte(0xff11, 0x80);
    ASSERT_EQ(apu.readByte(0xff11), 0x80);
}