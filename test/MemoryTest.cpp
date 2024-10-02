#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../test/MockGraphics.h"
#include "../test/MockAudio.h"
#include "../src/Memory.h"

using ::testing::Range;

TEST(MemoryTest, noFileReadByteWorks)
{
    Memory mmu;
    ASSERT_EQ(mmu.readByte(0), 0);
    ASSERT_EQ(mmu.readByte(1), 0);
    ASSERT_EQ(mmu.readByte(2), 0);
}

TEST(MemoryTest, noFileReadWordWorks)
{
    Memory mmu;
    ASSERT_EQ(mmu.readWord(1), 0);
}

TEST(MemoryTest, testFileReadByteWorks)
{
    Memory mmu = Memory("../test/test.bin");
    ASSERT_EQ(mmu.readByte(0), 0x31);
    ASSERT_EQ(mmu.readByte(1), 0xFE);
    ASSERT_EQ(mmu.readByte(2), 0xFF);
}

TEST(MemoryTest, testFileReadWordWorks)
{
    Memory mmu = Memory("../test/test.bin");
    ASSERT_EQ(mmu.readWord(1), 0xFFFE);
}

class GraphicsMemoryTest : public testing::TestWithParam<int>
{
};

TEST_P(GraphicsMemoryTest, writeByte)
{
    MockGraphics gpu;
    Memory mmu(&gpu);
    EXPECT_CALL(gpu, writeByte(GetParam(), 0x80))
        .Times(1);
    mmu.writeByte(GetParam(), 0x80);
}

INSTANTIATE_TEST_SUITE_P(Vram, GraphicsMemoryTest, Range(0x8000, 0xA000, 16));
INSTANTIATE_TEST_SUITE_P(, GraphicsMemoryTest, Range(0xFF47, 0xFF48));

class AudioMemoryTest : public testing::TestWithParam<int>
{
};

TEST_P(AudioMemoryTest, writeByte)
{
    MockAudio apu;
    Memory mmu(&apu);
    EXPECT_CALL(apu, writeByte(GetParam(), 0x80))
        .Times(1);
    mmu.writeByte(GetParam(), 0x80);
}

INSTANTIATE_TEST_SUITE_P(, AudioMemoryTest, Range(0xFF10, 0xFF30));
