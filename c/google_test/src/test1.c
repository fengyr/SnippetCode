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

int foo_false()
{
    return 0;
}

int foo_true()
{
    return 0;
}

int bar_true()
{
    return 1;
}

int bar_false()
{
    return 0;
}

int lee_true()
{
    return 1;
}

int lee_false()
{
    return 0;
}

TEST(lee, Negative) {
    EXPECT_TRUE(lee_true());
    EXPECT_FALSE(lee_false());
}

TEST(bar, Trivial) {
    EXPECT_TRUE(foo_true());
    EXPECT_FALSE(foo_false());
}

TEST(foo, Positive) {
    EXPECT_TRUE(bar_true());
    EXPECT_FALSE(bar_false());
}
