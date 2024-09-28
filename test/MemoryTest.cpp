#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../test/MockGraphics.h"
#include "../test/MockAudio.h"
#include "../src/Memory.h"

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

TEST(MemoryTest, writeByteGpuWorks)
{
    MockGraphics gpu;
    Memory mmu(&gpu);
    EXPECT_CALL(gpu, writeByte(0x9fff, 0))
        .Times(1);
    EXPECT_CALL(gpu, writeByte(0x8fff, 0))
        .Times(1);

    mmu.writeByte(0x9fff, 0);
    mmu.writeByte(0x8fff, 0);
}

TEST(MemoryTest, writeByteApuWorks)
{
    MockAudio apu;
    Memory mmu(&apu);
    EXPECT_CALL(apu, writeByte(0xff26, 0x80))
        .Times(1);
    EXPECT_CALL(apu, writeByte(0xff11, 0x80))
        .Times(1);

    mmu.writeByte(0xff26, 0x80);
    mmu.writeByte(0xff11, 0x80);
}