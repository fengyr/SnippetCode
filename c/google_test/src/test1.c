/*
 * =====================================================================================
 *
 *       Filename:  test1.c
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  一月 28, 2015
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "limits.h"
#include "gtest/gtest.h"

int foo_true()
{
    return 1;
}

int foo_false()
{
    return 0;
}

void* bar_point()
{
    static int i;

    return &i;
}

int lee(int r)
{
    return r;
}

TEST(TestSuit, TestLee) {
    int r = 0;
    EXPECT_EQ(lee(1), r) << " arg = " << r;

    r = 1;
    EXPECT_EQ(lee(0), r) << " arg = " << r;
}

TEST(TestSuit, TestBar) {
    EXPECT_TRUE(bar_point() != NULL);
}

TEST(TestSuit, TestFoo) {
    EXPECT_TRUE(foo_true());
    EXPECT_FALSE(foo_false());
}
