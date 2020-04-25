#include "string_matching/kmp.hpp"

#include <string>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace dagomez {
namespace {

TEST(KmpTest, NoPossibleMatch) {
  EXPECT_EQ(find("this is my text", "fail"), std::string::npos);
}

TEST(KmpTest, EmptyPattern) {
  EXPECT_EQ(find("this is my text", ""), 0);
}

TEST(KmpTest, EmptyText) {
  EXPECT_EQ(find("", "pattern"), std::string::npos);
}

TEST(KmpTest, SimpleMatch) {
  EXPECT_EQ(find("this is my text", "his"), 1);
  EXPECT_EQ(find("this is my text", "my"), 8);
}

TEST(KmpTest, MakeSureIsFirstMatch) {
  EXPECT_EQ(find("this is my text", "is"), 2);
}

} // namespace
} // namespace dagomez
