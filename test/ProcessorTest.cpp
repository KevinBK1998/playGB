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

TEST(ProcessorTest, lddAtHLFromA)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setA(0);
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
    ASSERT_EQ(cpu.getMachineCycles(), 2);
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
    ASSERT_EQ(cpu.getMachineCycles(), 2);
}

TEST(ProcessorTest, loadDataAtHighCFromA)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setA(0);
    cpu.setC(0x11);
    EXPECT_CALL(mmu, writeByte(0xff11, 0))
        .Times(1);

    cpu.map(0xE2);

    ASSERT_EQ(cpu.getPC(), 0);
}

TEST(ProcessorTest, incC)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setF(0);
    cpu.setC(0x11);

    cpu.map(0xC);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getC(), 0x12);
    ASSERT_EQ(cpu.getF(), 0);
}

TEST(ProcessorTest, loadDataAtHLFromA)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setA(0x77);
    cpu.setHL(1);
    EXPECT_CALL(mmu, writeByte(1, 0x77))
        .Times(1);

    cpu.map(0x77);

    ASSERT_EQ(cpu.getHL(), 1);
    ASSERT_EQ(cpu.getPC(), 0);
}

TEST(ProcessorTest, loadDataAtHNFromA)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setA(0xFC);
    EXPECT_CALL(mmu, readByte(0))
        .Times(1)
        .WillOnce(Return(0x47));
    EXPECT_CALL(mmu, writeByte(0xFF47, 0xFC))
        .Times(1);

    cpu.map(0xE0);

    ASSERT_EQ(cpu.getPC(), 1);
}

TEST(ProcessorTest, load_de_From_nn)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, readWord(0))
        .Times(1)
        .WillOnce(Return(0x104));

    cpu.map(0x11);

    ASSERT_EQ(cpu.getDE(), 0x104);
    ASSERT_EQ(cpu.getPC(), 2);
}

TEST(ProcessorTest, loadDataAtAFromDE)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setDE(0x104);
    EXPECT_CALL(mmu, readByte(0x104))
        .Times(1)
        .WillOnce(Return(1));

    cpu.map(0x1A);

    ASSERT_EQ(cpu.getA(), 1);
    ASSERT_EQ(cpu.getPC(), 0);
}

TEST(ProcessorTest, callImmediate)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setPC(0x29);
    cpu.setSP(0xfffe);
    EXPECT_CALL(mmu, readWord(0x29))
        .Times(1)
        .WillOnce(Return(0x95));
    EXPECT_CALL(mmu, writeWord(0xFFFC, 0x2B))
        .Times(1);

    cpu.map(0xCD);

    ASSERT_EQ(cpu.getSP(), 0xfffc);
    ASSERT_EQ(cpu.getPC(), 0x95);
}

TEST(ProcessorTest, loadCFromA)
{
    Processor cpu;
    cpu.setA(0xF);

    cpu.map(0x4F);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getC(), 0xF);
    ASSERT_EQ(cpu.getMachineCycles(), 1);
}

TEST(ProcessorTest, loadBFromImmediate)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    EXPECT_CALL(mmu, readByte(0))
        .Times(1)
        .WillOnce(Return(0x11));

    cpu.map(0x6);

    ASSERT_EQ(cpu.getB(), 0x11);
    ASSERT_EQ(cpu.getPC(), 1);
    ASSERT_EQ(cpu.getMachineCycles(), 2);
}

TEST(ProcessorTest, pushBC)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setSP(0xfffe);
    cpu.setBC(0x400);
    EXPECT_CALL(mmu, writeWord(0xFFFC, 0x400))
        .Times(1);

    cpu.map(0xC5);

    ASSERT_EQ(cpu.getSP(), 0xfffc);
    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getMachineCycles(), 3);
}

// Prefix tests start here

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
    cpu.setF(0);
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

TEST(ProcessorTest, rotateLeftC)
{
    Processor cpu;
    cpu.setC(0);
    cpu.setF(0);

    cpu.prefixMap(0x11);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getC(), 0);
    ASSERT_EQ(cpu.getF(), 0x80);
    ASSERT_EQ(cpu.getMachineCycles(), 2);

    cpu.setC(0);
    cpu.setF(0x10);

    cpu.prefixMap(0x11);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getC(), 1);
    ASSERT_EQ(cpu.getF(), 0);

    cpu.setC(0xFF);
    cpu.setF(0);

    cpu.prefixMap(0x11);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getC(), 0xFE);
    ASSERT_EQ(cpu.getF(), 0x10);

    cpu.setC(0xFF);
    cpu.setF(0x10);

    cpu.prefixMap(0x11);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getC(), 0xFF);
    ASSERT_EQ(cpu.getF(), 0x10);
}

TEST(ProcessorTest, rotateLeftA)
{
    Processor cpu;
    cpu.setA(0);
    cpu.setF(0);

    cpu.map(0x17);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getA(), 0);
    ASSERT_EQ(cpu.getF(), 0x80);
    ASSERT_EQ(cpu.getMachineCycles(), 1);

    cpu.setA(0);
    cpu.setF(0x10);

    cpu.map(0x17);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getA(), 1);
    ASSERT_EQ(cpu.getF(), 0);

    cpu.setA(0xFF);
    cpu.setF(0);

    cpu.map(0x17);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getA(), 0xFE);
    ASSERT_EQ(cpu.getF(), 0x10);

    cpu.setA(0xFF);
    cpu.setF(0x10);

    cpu.map(0x17);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getA(), 0xFF);
    ASSERT_EQ(cpu.getF(), 0x10);
}

TEST(ProcessorTest, popBC)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setSP(0xfffa);
    cpu.setBC(0);
    EXPECT_CALL(mmu, readWord(0xFFFA))
        .Times(1)
        .WillOnce(Return(0x400));

    cpu.map(0xC1);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getSP(), 0xfffc);
    ASSERT_EQ(cpu.getBC(), 0x400);
    ASSERT_EQ(cpu.getMachineCycles(), 3);
}

TEST(ProcessorTest, decB)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setF(0);
    cpu.setBC(0x400);

    cpu.map(0x5);

    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getBC(), 0x300);
    ASSERT_EQ(cpu.getF(), 0);
}

TEST(ProcessorTest, ldiHLFromA)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setA(0);
    cpu.setHL(0);
    EXPECT_CALL(mmu, writeByte(0, 0))
        .Times(1);

    cpu.map(0x22);

    ASSERT_EQ(cpu.getHL(), 1);
    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getMachineCycles(), 2);
}

TEST(ProcessorTest, inc_hl)
{
    Processor cpu;
    cpu.setHL(0);

    cpu.map(0x23);

    ASSERT_EQ(cpu.getHL(), 1);
    ASSERT_EQ(cpu.getPC(), 0);
    ASSERT_EQ(cpu.getMachineCycles(), 1);
}

TEST(ProcessorTest, ret)
{
    MockMemory mmu;
    Processor cpu = Processor(&mmu);
    cpu.setPC(0xA7);
    cpu.setSP(0xfffc);
    EXPECT_CALL(mmu, readWord(0xFFFC))
        .Times(1)
        .WillOnce(Return(0x2B));

    cpu.map(0xC9);

    ASSERT_EQ(cpu.getSP(), 0xfffe);
    ASSERT_EQ(cpu.getPC(), 0x2B);
    ASSERT_EQ(cpu.getMachineCycles(), 3);
}
