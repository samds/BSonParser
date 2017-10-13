#include <iostream>
#include <cassert>
#include <gtest/gtest.h>
/*
int main()
{
  int answer = 40;
  answer += 2;
  if (answer != 42)
  {
    std::cerr << "We are all going to die!" << std::endl;
    return 2;
  }

  std::cout << "And the answer is: " << answer << std::endl;
  std::cout << "Ok." << std::endl;
  return 0;
}
 */

TEST(TestObject, ABI)
{
    int x = 42;
    int y = 43;
    EXPECT_EQ(42, x);
    EXPECT_EQ(42, y);
}

TEST(TestObject, ABB)
{
    int x = 42;
    int y = 43;
    EXPECT_EQ(42, x);
    EXPECT_EQ(42, y);
}
