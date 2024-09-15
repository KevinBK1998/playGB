#include <gtest/gtest.h>
#include "../src/Memory.h"

TEST(MemoryTest, noFileReadWorks)
{
    Memory mmu;
    ASSERT_EQ(mmu.read(0), 0);
    ASSERT_EQ(mmu.read(1), 0);
}

TEST(MemoryTest, testFileReadWorks)
{
    Memory mmu = Memory("../test/test.bin");
    ASSERT_EQ(mmu.read(0), 0x31);
    ASSERT_EQ(mmu.read(1), 0xFE);
}