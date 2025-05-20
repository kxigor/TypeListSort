#include <gtest/gtest.h>
#include <type_traits>
#include "TypeList.hpp"

using namespace type_list;

TEST(TypeListTest, Head) {
  using List = TypeList<int, double, char>;
  using FirstType = typename Head<List>::type;
  static_assert(std::is_same_v<FirstType, int>, "Head should be int");
}

TEST(TypeListTest, Tail) {
  using List = TypeList<int, double, char>;
  using RestTypes = typename Tail<List>::type;
  static_assert(std::is_same_v<RestTypes, TypeList<double, char>>, "Tail should be TypeList<double, char>");
}

TEST(TypeListTest, Get) {
  using List = TypeList<int, double, char>;
  using Elem0 = typename Get<0, List>::type;
  using Elem1 = typename Get<1, List>::type;
  using Elem2 = typename Get<2, List>::type;

  static_assert(std::is_same_v<Elem0, int>, "Get<0> should be int");
  static_assert(std::is_same_v<Elem1, double>, "Get<1> should be double");
  static_assert(std::is_same_v<Elem2, char>, "Get<2> should be char");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}