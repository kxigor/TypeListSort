#include <gtest/gtest.h>

#include <type_traits>

#include "TypeList.hpp"
#include "TypeListSort.hpp"
#include "TypesPredicates.hpp"

using namespace type_list;
using namespace type_list::sort;
using namespace type_list::predicates;

struct SmallType {
  char c;
};

struct MediumType {
  int i;
};

struct LargeType {
  double d;
  int i;
};

using Permutations =
    ::testing::Types<TypeList<SmallType, MediumType, LargeType>,
                     TypeList<SmallType, LargeType, MediumType>,
                     TypeList<MediumType, SmallType, LargeType>,
                     TypeList<MediumType, LargeType, SmallType>,
                     TypeList<LargeType, SmallType, MediumType>,
                     TypeList<LargeType, MediumType, SmallType>>;

template <typename T>
class BubbleSortTest : public ::testing::Test {};
TYPED_TEST_SUITE(BubbleSortTest, Permutations);

TYPED_TEST(BubbleSortTest, SortsAnyPermutationBySize) {
  using Input = TypeParam;
  using Sorted = BubbleSortT<SizeLessPred, Input>;
  static_assert(
      IsSameListV<Sorted, TypeList<SmallType, MediumType, LargeType>>);
}

TEST(BubbleSortBoundaryTest, EmptyList) {
  using Empty = TypeList<>;
  using Sorted = BubbleSort<SizeLessPred, Empty>::type;
  static_assert(IsSameListV<Sorted, TypeList<>>,
                "Empty list should remain empty");
}

TEST(BubbleSortBoundaryTest, SingleElement) {
  using Single = TypeList<MediumType>;
  using Sorted = BubbleSort<SizeLessPred, Single>::type;
  static_assert(IsSameListV<Sorted, TypeList<MediumType>>,
                "Single-element list should stay unchanged");
}

TEST(AlignPredicatesTest, SortsByAlignment) {
  struct A { char c; }; // alignof = 1
  struct B { int i; };  // alignof = 4
  struct C  { alignas(16) double d; }; // alignof = 16

  using List = TypeList<B, C, A>;
  using SortedAsc = BubbleSort<AlignLessPred, List>::type;
  using SortedDesc = BubbleSort<AlignGreaterPred, List>::type;

  static_assert(std::is_same_v<SortedAsc, TypeList<A, B, C>>);
  static_assert(std::is_same_v<SortedDesc, TypeList<C, B, A>>);
}

TEST(IsBaseOfPredTest, SortsByInheritance) {
  struct Base {};
  struct Derived : Base {};
  struct Unrelated {};

  using List = TypeList<Unrelated, Derived, Base>;
  using Sorted = BubbleSort<IsBaseOfPred, List>::type;

  // Base должен быть перед Derived
  static_assert(std::is_same_v<Sorted, TypeList<Base, Derived, Unrelated>> ||
                std::is_same_v<Sorted, TypeList<Base, Unrelated, Derived>>);
}

TEST(IsConstructiblePredTest, SortsByConstructibility) {
  struct A { A(int) {} };
  struct B { B(double) {} };
  struct C { C(A) {} };

  using List = TypeList<B, C, A>;
  using Sorted = BubbleSort<IsConstructiblePred, List>::type;

  // A можно построить из int, C из A - особый порядок
  static_assert(std::is_same_v<HeadT<Sorted>, A>);
}

TEST(ConvertsBetterPredTest, SortsByConvertibility) {
  struct A { operator int() const { return 0; } };
  struct B { explicit operator double() const { return 0.0; } };

  using List = TypeList<B, A>;
  using Sorted = BubbleSort<ConvertsBetterPred, List>::type;

  // A лучше конвертируется в числа
  static_assert(std::is_same_v<Sorted, TypeList<A, B>>);
}

TEST(IsMoreDerivedPredTest, SortsByDerivationDepth) {
  struct Base {};
  struct Mid : Base {};
  struct Top : Mid {};

  using List = TypeList<Base, Top, Mid>;
  using Sorted = BubbleSort<IsMoreDerivedPred, List>::type;

  static_assert(std::is_same_v<HeadT<Sorted>, Top>);
}

TEST(DepthOfInheritancePredTest, SortsByInheritanceDepth) {
  struct L1 {};
  struct L2 : L1 {};
  struct L3 : L2 {};
  struct L2_2 : L1 {};

  using List = TypeList<L1, L3, L2_2>;
  using SortedShallowFirst = BubbleSort<DepthOfInheritanceLessPred, List>::type;
  using SortedDeepFirst = BubbleSort<DepthOfInheritanceGreaterPred, List>::type;

  static_assert(std::is_same_v<HeadT<SortedShallowFirst>, L1>);
  static_assert(std::is_same_v<HeadT<SortedDeepFirst>, L3>);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}