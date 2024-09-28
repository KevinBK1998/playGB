#include <gtest/gtest.h>
#include "../src/Graphics.h"

TEST(GraphicsTest, writeVram)
{
    Graphics gpu;
    gpu.writeByte(0x9fff, 0x80);
    ASSERT_EQ(gpu.readByte(0x9fff), 0x80);
    gpu.writeByte(0x9fff, 0x80);
    ASSERT_EQ(gpu.readByte(0x9fff), 0x80);
}
