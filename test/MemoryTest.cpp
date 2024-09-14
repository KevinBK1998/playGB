#include <gtest/gtest.h>
#include "../src/Memory.h"

TEST(MemoryTest, load)
{
    ASSERT_EQ(load("rom/bios.gb"), 0x31);
}
