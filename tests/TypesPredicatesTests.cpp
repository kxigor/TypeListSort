#include <gtest/gtest.h>

#include <cstdint>

#include "TypesPredicates.hpp"

struct Base {};
struct Derived : Base {};
struct MoreDerived : Derived {};
struct Unrelated {};

struct ConstructibleFromInt {
  ConstructibleFromInt(int) {}
};
struct NotConstructibleFromInt {};

struct Size4Align4 {
  int x;
};
struct Size8Align4 {
  alignas(4) int x;
  int y;
};
struct Size16Align8 {
  alignas(8) double x;
  double y;
};
struct Size32Align16 {
  alignas(16) double x[4];
};

TEST(PredicatesTest, SizeLessPred) {
  using namespace type_list::predicates;

  static_assert(type_list::predicates::SizeLessPred<char, int>::value);
  static_assert(type_list::predicates::SizeLessPred<int, double>::value);
  static_assert(!type_list::predicates::SizeLessPred<double, int>::value);
  static_assert(!type_list::predicates::SizeLessPred<int, char>::value);
  static_assert(!type_list::predicates::SizeLessPred<int, int>::value);
}

TEST(PredicatesTest, SizeGreaterPred) {
  using namespace type_list::predicates;

  static_assert(type_list::predicates::SizeGreaterPred<int, char>::value);
  static_assert(type_list::predicates::SizeGreaterPred<double, int>::value);
  static_assert(!type_list::predicates::SizeGreaterPred<char, int>::value);
  static_assert(!type_list::predicates::SizeGreaterPred<int, double>::value);
  static_assert(!type_list::predicates::SizeGreaterPred<int, int>::value);
}

TEST(PredicatesTest, AlignLessPred) {
  using namespace type_list::predicates;

  static_assert(
      type_list::predicates::AlignLessPred<Size4Align4, Size16Align8>::value);
  static_assert(
      type_list::predicates::AlignLessPred<Size8Align4, Size16Align8>::value);
  static_assert(
      !type_list::predicates::AlignLessPred<Size16Align8, Size4Align4>::value);
  static_assert(
      !type_list::predicates::AlignLessPred<Size16Align8, Size16Align8>::value);
}

TEST(PredicatesTest, AlignGreaterPred) {
  using namespace type_list::predicates;

  static_assert(type_list::predicates::AlignGreaterPred<Size16Align8,
                                                        Size4Align4>::value);
  static_assert(type_list::predicates::AlignGreaterPred<Size32Align16,
                                                        Size8Align4>::value);
  static_assert(!type_list::predicates::AlignGreaterPred<Size4Align4,
                                                         Size16Align8>::value);
  static_assert(!type_list::predicates::AlignGreaterPred<Size16Align8,
                                                         Size16Align8>::value);
}

TEST(PredicatesTest, IsBaseOfPred) {
  using namespace type_list::predicates;

  static_assert(type_list::predicates::IsBaseOfPred<Base, Derived>::value);
  static_assert(type_list::predicates::IsBaseOfPred<Base, MoreDerived>::value);
  static_assert(!type_list::predicates::IsBaseOfPred<Derived, Base>::value);
  static_assert(!type_list::predicates::IsBaseOfPred<Base, Unrelated>::value);
  static_assert(type_list::predicates::IsBaseOfPred<Base, Base>::value);
}

TEST(PredicatesTest, IsConstructiblePred) {
  using namespace type_list::predicates;

  static_assert(
      type_list::predicates::IsConstructiblePred<int,
                                                 ConstructibleFromInt>::value);
  static_assert(!type_list::predicates::IsConstructiblePred<
                int, NotConstructibleFromInt>::value);
  static_assert(
      type_list::predicates::IsConstructiblePred<double, double>::value);
}

TEST(PredicatesTest, ConvertsBetterPred) {
  using namespace type_list::predicates;

  static_assert(type_list::predicates::ConvertsBetterPred<int, double>::value);
  static_assert(
      type_list::predicates::ConvertsBetterPred<Derived, Base>::value);
  static_assert(
      !type_list::predicates::ConvertsBetterPred<Base, Derived>::value);
  static_assert(
      !type_list::predicates::ConvertsBetterPred<Unrelated, Base>::value);
  static_assert(type_list::predicates::ConvertsBetterPred<int, int>::value);
}

TEST(PredicatesTest, IsMoreDerivedPred) {
  using namespace type_list::predicates;

  static_assert(type_list::predicates::IsMoreDerivedPred<Derived, Base>::value);
  static_assert(
      type_list::predicates::IsMoreDerivedPred<MoreDerived, Base>::value);
  static_assert(
      type_list::predicates::IsMoreDerivedPred<MoreDerived, Derived>::value);
  static_assert(
      !type_list::predicates::IsMoreDerivedPred<Base, Derived>::value);
  static_assert(!type_list::predicates::IsMoreDerivedPred<Base, Base>::value);
  static_assert(
      !type_list::predicates::IsMoreDerivedPred<Unrelated, Base>::value);
}

TEST(PredicatesTest, TypeNameLessPred) {
  using namespace type_list::predicates;

  constexpr bool int_less_double = TypeNameLessPred<int, double>::value;
  constexpr bool double_less_int = TypeNameLessPred<double, int>::value;
  static_assert(int_less_double || double_less_int);
  static_assert(not(int_less_double && double_less_int));

  static_assert(not TypeNameLessPred<int, int>::value);
}

TEST(PredicatesTest, TypeNameGreaterPred) {
  using namespace type_list::predicates;

  static_assert(TypeNameLessPred<char, double>::value);
  static_assert(!TypeNameLessPred<double, char>::value);

  static_assert(TypeNameLessPred<double, float>::value);
  static_assert(!TypeNameLessPred<float, double>::value);

  static_assert(TypeNameLessPred<int, short>::value);
  static_assert(!TypeNameLessPred<short, int>::value);

  static_assert(TypeNameLessPred<long, short>::value);
  static_assert(!TypeNameLessPred<short, long>::value);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}