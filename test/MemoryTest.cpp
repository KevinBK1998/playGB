#include <gtest/gtest.h>
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

TEST(MemoryTest, writeByteVramWorks)
{
    Memory mmu;
    mmu.writeByte(0x9fff, 0);
    ASSERT_EQ(mmu.readByte(0x9fff), 0);
    mmu.writeByte(0x8fff, 0);
    ASSERT_EQ(mmu.readByte(0x8fff), 0);
}

TEST(MemoryTest, writeByteApuWorks)
{
    Memory mmu;
    mmu.writeByte(0xff26, 0x80);
    ASSERT_EQ(mmu.readByte(0xff26), 0x80);
}