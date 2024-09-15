#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../test/MockMemory.h"
#include "../src/Processor.h"

using ::testing::Return;

TEST(ProcessorTest, testMemoryReadCalled)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, read(0)).Times(1).WillOnce(Return(0x31));
    EXPECT_CALL(mmu, read(1)).Times(1).WillOnce(Return(0xFE));

    ASSERT_EQ(cpu.read(0), 0x31);
    ASSERT_EQ(cpu.read(1), 0xFE);
}

TEST(ProcessorTest, pcShouldBeZeroOnStart)
{
    Processor cpu;
    ASSERT_EQ(cpu.getPC(), 0);
}

TEST(ProcessorTest, spShouldBeZeroOnStart)
{
    Processor cpu;
    ASSERT_EQ(cpu.getSP(), 0);
}
