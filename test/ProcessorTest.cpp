#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../test/MockMemory.h"
#include "../src/Processor.h"

using ::testing::Return;

TEST(ProcessorTest, testMemoryReadCalled)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, readByte(0))
        .Times(1)
        .WillOnce(Return(0x31));
    EXPECT_CALL(mmu, readByte(1))
        .Times(1)
        .WillOnce(Return(0xFE));

    ASSERT_EQ(cpu.read(0), 0x31);
    ASSERT_EQ(cpu.read(1), 0xFE);
}

TEST(ProcessorTest, registerShouldBeZeroOnStart)
{
    Processor cpu;
    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getSP(), 0);
}

TEST(ProcessorTest, stepShouldincreasePC)
{
    Processor cpu;
    cpu.step();
    ASSERT_EQ(cpu.getPC(), 1);
    ASSERT_EQ(cpu.getSP(), 0);
}

TEST(ProcessorTest, testNopWorks)
{
    Processor cpu;
    cpu.map(0);
    ASSERT_EQ(cpu.getSP(), 0);
    ASSERT_EQ(cpu.getPC(), 0);
}

TEST(ProcessorTest, loadSPFromPC)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, readWord(0))
        .Times(1)
        .WillOnce(Return(0xFFFE));

    cpu.map(0x31);

    ASSERT_EQ(cpu.getSP(), 0xFFFE);
    ASSERT_EQ(cpu.getPC(), 2);
}

TEST(ProcessorTest, loadHLFromPC)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, readWord(0))
        .Times(1)
        .WillOnce(Return(0xFFFE));

    cpu.map(0x21);

    ASSERT_EQ(cpu.getHL(), 0xFFFE);
    ASSERT_EQ(cpu.getPC(), 2);
}

TEST(ProcessorTest, xorA)
{
    Processor cpu = Processor();
    cpu.map(0xAF);
    ASSERT_EQ(cpu.getA(), 0);
    ASSERT_EQ(cpu.getF(), 0x80);
    ASSERT_EQ(cpu.getPC(), 0);
}

TEST(ProcessorTest, loadDataAtHLFromA)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.map(0xAF);
    cpu.setHL(1);
    EXPECT_CALL(mmu, writeByte(1, 0))
        .Times(1);

    cpu.map(0x32);

    ASSERT_EQ(cpu.read(1), 0);
    ASSERT_EQ(cpu.getHL(), 0);
    ASSERT_EQ(cpu.getPC(), 0);
}
