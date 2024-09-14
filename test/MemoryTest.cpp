#include <gtest/gtest.h>
#include "../src/Memory.h"

TEST(MemoryTest, load)
{
    ASSERT_EQ(load("../test/test.bin"), 0x31);
}
