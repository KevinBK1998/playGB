#include <gtest/gtest.h>
#include "../src/myFunctions.h"

TEST(myFunctions, add)
{
    // GTEST_ASSERT_EQ(add(10, 22), 32);
    GTEST_ASSERT_EQ(factorial(5), 120);
}
