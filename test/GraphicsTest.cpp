#include <gtest/gtest.h>
#include "../src/Graphics.h"

using ::testing::Range;

class GraphicsTest : public testing::TestWithParam<int>
{
};

TEST_P(GraphicsTest, writeVram)
{
    Graphics gpu;
    gpu.writeByte(GetParam(), 0x80);
    ASSERT_EQ(gpu.readByte(GetParam()), 0x80);
}

INSTANTIATE_TEST_SUITE_P(, GraphicsTest, Range(0x8000, 0xA000, 16));
