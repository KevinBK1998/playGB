#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../test/MockMemory.h"
#include "../src/Processor.h"

using ::testing::Return;

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

TEST(ProcessorTest, stepShouldCallMemoryRead)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, readByte(0))
        .Times(1)
        .WillOnce(Return(0x31));
    EXPECT_CALL(mmu, readWord(1))
        .Times(1)
        .WillOnce(Return(0xFFFE));

    cpu.step();

    ASSERT_EQ(cpu.getPC(), 3);
    ASSERT_EQ(cpu.getSP(), 0xFFFE);
}

TEST(ProcessorTest, testNopWorks)
{
    Processor cpu;
    cpu.map(0);
    ASSERT_EQ(cpu.getSP(), 0);
    ASSERT_EQ(cpu.getPC(), 0);
}

TEST(ProcessorTest, loadSPFromImm)
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

TEST(ProcessorTest, loadHLFromImm)
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

    ASSERT_EQ(cpu.getHL(), 0);
    ASSERT_EQ(cpu.getPC(), 0);
}

TEST(ProcessorTest, jumpRelative)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setPC(0xA);
    EXPECT_CALL(mmu, readByte(0xA))
        .Times(1)
        .WillOnce(Return(0x20));
    EXPECT_CALL(mmu, readByte(0xB))
        .Times(1)
        .WillOnce(Return(0xFB));

    cpu.step();

    ASSERT_EQ(cpu.getPC(), 7);
}

TEST(ProcessorTest, loadAFromImm)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, readByte(0))
        .Times(1)
        .WillOnce(Return(0x80));

    cpu.map(0x3E);

    ASSERT_EQ(cpu.getA(), 0x80);
    ASSERT_EQ(cpu.getPC(), 1);
}

TEST(ProcessorTest, loadCFromImm)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, readByte(0))
        .Times(1)
        .WillOnce(Return(0x11));

    cpu.map(0xE);

    ASSERT_EQ(cpu.getC(), 0x11);
    ASSERT_EQ(cpu.getPC(), 1);
}

TEST(ProcessorTest, prefixOpcodesIncreasePC)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, readByte(0))
        .Times(1)
        .WillOnce(Return(0xCB));
    EXPECT_CALL(mmu, readByte(1))
        .Times(1)
        .WillOnce(Return(0x7C));

    cpu.step();

    ASSERT_EQ(cpu.getPC(), 2);
    ASSERT_EQ(cpu.getSP(), 0);
}

TEST(ProcessorTest, prefixBitCheck)
{
    Processor cpu;
    cpu.map(0xAF);
    cpu.setHL(0xFFFF);

    cpu.prefixMap(0x7C);
    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getHL(), 0xFFFF);
    ASSERT_EQ(cpu.getF(), 0x20);

    cpu.setHL(0);

    cpu.prefixMap(0x7C);
    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getHL(), 0);
    ASSERT_EQ(cpu.getF(), 0xA0);
}
