#include <gtest/gtest.h>
#include "../src/Memory.h"

class MemoryTest : public testing::Test
{
protected:
    Memory mmu = Memory("../test/test.bin");
};

TEST_F(MemoryTest, readWorks)
{
    ASSERT_EQ(mmu.read(0), 0x31);
    ASSERT_EQ(mmu.read(1), 0xFE);
}