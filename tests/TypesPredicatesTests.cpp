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


struct A {}; // depth = 0
struct B : A {}; // depth = 1
struct C : B {}; // depth = 2
struct D : C {}; // depth = 3
struct E : D {}; // depth = 4

struct A2 {}; // depth = 0
struct B2 : A2 {}; // depth = 1
struct C2 : B2 {}; // depth = 2
struct D2 : C2 {}; // depth = 3
struct E2 : D2 {}; // depth = 4

struct F : B, C2 {}; // depth = max(1, 2) + 1 = 3
struct G : F, D2 {}; // depth = max(3, 3) + 1 = 4
struct H : G, E2 {}; // depth = max(4, 4) + 1 = 5

TEST(DepthOfInheritancePredTest, BasicCases) {
  using namespace type_list::predicates;

  static_assert(DepthOfInheritanceLessPred<A, B>::value, "A < B");
  static_assert(DepthOfInheritanceLessPred<B, C>::value, "B < C");
  static_assert(DepthOfInheritanceLessPred<C, D>::value, "C < D");
  
  static_assert(DepthOfInheritanceGreaterPred<D, C>::value, "D > C");
  static_assert(DepthOfInheritanceGreaterPred<C, B>::value, "C > B");
  static_assert(DepthOfInheritanceGreaterPred<B, A>::value, "B > A");
}

TEST(DepthOfInheritancePredTest, EqualDepth) {
  using namespace type_list::predicates;

  static_assert(!DepthOfInheritanceLessPred<D, D>::value, "D !< D");
  static_assert(!DepthOfInheritanceGreaterPred<D, D>::value, "D !> D");
  
  struct X : A {}; // depth = 1
  static_assert(!DepthOfInheritanceLessPred<B, X>::value, "B !< X");
  static_assert(!DepthOfInheritanceGreaterPred<B, X>::value, "B !> X");
}

TEST(DepthOfInheritancePredTest, MultipleInheritance) {
  using namespace type_list::predicates;

  static_assert(DepthOfInheritanceLessPred<B, F>::value, "B < F");
  static_assert(DepthOfInheritanceLessPred<C, F>::value, "C < F");
  static_assert(DepthOfInheritanceLessPred<F, G>::value, "F < G");
  static_assert(DepthOfInheritanceLessPred<G, H>::value, "G < H");
  
  static_assert(DepthOfInheritanceGreaterPred<H, G>::value, "H > G");
  static_assert(DepthOfInheritanceGreaterPred<G, F>::value, "G > F");
  static_assert(DepthOfInheritanceGreaterPred<F, C>::value, "F > C");
}

TEST(DepthOfInheritancePredTest, ComplexCases) {
  using namespace type_list::predicates;

  struct X : A {}; // depth = 1
  struct Y : X, B {}; // depth = max(1, 1) + 1 = 2
  struct Z : Y, C {}; // depth = max(2, 2) + 1 = 3
  
  static_assert(DepthOfInheritanceLessPred<Y, Z>::value, "Y < Z");
  static_assert(DepthOfInheritanceLessPred<X, Y>::value, "X < Y");
  static_assert(DepthOfInheritanceLessPred<A, Z>::value, "A < Z");
  
  static_assert(DepthOfInheritanceGreaterPred<Z, Y>::value, "Z > Y");
  static_assert(DepthOfInheritanceGreaterPred<Y, X>::value, "Y > X");
  static_assert(DepthOfInheritanceGreaterPred<Z, A>::value, "Z > A");
}

TEST(DepthOfInheritancePredTest, MixedCases) {
  using namespace type_list::predicates;

  struct X {}; // depth = 0
  struct X2 {}; // depth = 0
  struct Y : X {}; // depth = 1
  struct Z : X2, Y {}; // depth = max(0, 1) + 1 = 2
  
  static_assert(DepthOfInheritanceLessPred<X, Y>::value, "X < Y");
  static_assert(DepthOfInheritanceLessPred<Y, Z>::value, "Y < Z");
  static_assert(DepthOfInheritanceLessPred<A, Z>::value, "A < Z");
  
  static_assert(DepthOfInheritanceGreaterPred<Z, Y>::value, "Z > Y");
  static_assert(DepthOfInheritanceGreaterPred<Y, X>::value, "Y > X");
  static_assert(DepthOfInheritanceGreaterPred<Z, A>::value, "Z > A");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}