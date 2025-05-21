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
  static_assert(std::is_same_v<RestTypes, TypeList<double, char>>,
                "Tail should be TypeList<double, char>");
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

TEST(ConcatTest, EmptyLists) {
  using List1 = TypeList<>;
  using List2 = TypeList<>;
  using Result = typename Concat<List1, List2>::type;

  static_assert(std::is_same_v<Result, TypeList<>>,
                "Concat of two empty lists should be empty");
}

TEST(ConcatTest, FirstEmpty) {
  using List1 = TypeList<>;
  using List2 = TypeList<int, double>;
  using Result = typename Concat<List1, List2>::type;

  static_assert(std::is_same_v<Result, TypeList<int, double>>,
                "Concat with first list empty should return the second list");
}

TEST(ConcatTest, SecondEmpty) {
  using List1 = TypeList<char, float>;
  using List2 = TypeList<>;
  using Result = typename Concat<List1, List2>::type;

  static_assert(std::is_same_v<Result, TypeList<char, float>>,
                "Concat with second list empty should return the first list");
}

TEST(ConcatTest, NonEmptyLists) {
  using List1 = TypeList<int, double>;
  using List2 = TypeList<char, float>;
  using Result = typename Concat<List1, List2>::type;

  static_assert(std::is_same_v<Result, TypeList<int, double, char, float>>,
                "Concat should combine lists in order");
}

TEST(ConcatTest, MultipleTypes) {
  using List1 = TypeList<int, double>;
  using List2 = TypeList<char, float, bool>;
  using Result = typename Concat<List1, List2>::type;

  static_assert(
      std::is_same_v<Result, TypeList<int, double, char, float, bool>>,
      "Concat should handle lists of different lengths");
}

TEST(AppendBackTest, EmptyList) {
  using List = TypeList<>;
  using Result = typename AppendBack<int, List>::type;
  static_assert(std::is_same_v<Result, TypeList<int>>,
                "AppendBack to empty list should create single-element list");
}

TEST(AppendBackTest, NonEmptyList) {
  using List = TypeList<double, char>;
  using Result = typename AppendBack<int, List>::type;
  static_assert(std::is_same_v<Result, TypeList<int, double, char>>,
                "AppendBack should add element to the front");
}

TEST(AppendFrontTest, EmptyList) {
  using List = TypeList<>;
  using Result = typename AppendFront<int, List>::type;
  static_assert(std::is_same_v<Result, TypeList<int>>,
                "AppendFront to empty list should create single-element list");
}

TEST(AppendFrontTest, NonEmptyList) {
  using List = TypeList<double, char>;
  using Result = typename AppendFront<int, List>::type;
  static_assert(std::is_same_v<Result, TypeList<double, char, int>>,
                "AppendFront should add element to the back");
}

TEST(PopBackTest, SingleElement) {
  using List = TypeList<int>;
  using Result = typename PopBack<List>::type;
  static_assert(std::is_same_v<Result, TypeList<>>,
                "PopBack from single-element list should return empty list");
}

TEST(PopBackTest, MultipleElements) {
  using List = TypeList<int, double, char>;
  using Result = typename PopBack<List>::type;
  static_assert(std::is_same_v<Result, TypeList<int, double>>,
                "PopBack should remove last element");
}

TEST(PopFrontTest, SingleElement) {
  using List = TypeList<int>;
  using Result = typename PopFront<List>::type;
  static_assert(std::is_same_v<Result, TypeList<>>,
                "PopFront from single-element list should return empty list");
}

TEST(PopFrontTest, MultipleElements) {
  using List = TypeList<int, double, char>;
  using Result = typename PopFront<List>::type;
  static_assert(std::is_same_v<Result, TypeList<double, char>>,
                "PopFront should remove first element");
}

TEST(DropFrontTest, DropZeroElements) {
  using List = TypeList<int, double, char>;
  using Result = typename DropFront<0, List>::type;
  static_assert(std::is_same_v<Result, List>,
                "DropFront<0> should return original list");
}

TEST(DropFrontTest, DropOneElement) {
  using List = TypeList<int, double, char>;
  using Result = typename DropFront<1, List>::type;
  static_assert(std::is_same_v<Result, TypeList<double, char>>,
                "DropFront<1> should remove first element");
}

TEST(DropFrontTest, DropMultipleElements) {
  using List = TypeList<int, double, char, float>;
  using Result = typename DropFront<2, List>::type;
  static_assert(std::is_same_v<Result, TypeList<char, float>>,
                "DropFront<2> should remove first two elements");
}

TEST(DropFrontTest, DropAllElements) {
  using List = TypeList<int, double, char>;
  using Result = typename DropFront<3, List>::type;
  static_assert(std::is_same_v<Result, TypeList<>>,
                "DropFront<size> should return empty list");
}

TEST(DropBackTest, DropZeroElements) {
  using List = TypeList<int, double, char>;
  using Result = typename DropBack<0, List>::type;
  static_assert(std::is_same_v<Result, List>,
                "DropBack<0> should return original list");
}

TEST(DropBackTest, DropOneElement) {
  using List = TypeList<int, double, char>;
  using Result = typename DropBack<1, List>::type;
  static_assert(std::is_same_v<Result, TypeList<int, double>>,
                "DropBack<1> should remove last element");
}

TEST(DropBackTest, DropMultipleElements) {
  using List = TypeList<int, double, char, float>;
  using Result = typename DropBack<2, List>::type;
  static_assert(std::is_same_v<Result, TypeList<int, double>>,
                "DropBack<2> should remove last two elements");
}

TEST(DropBackTest, DropAllElements) {
  using List = TypeList<int, double, char>;
  using Result = typename DropBack<3, List>::type;
  static_assert(std::is_same_v<Result, TypeList<>>,
                "DropBack<size> should return empty list");
}

TEST(SliceTest, FullRange) {
  using List = TypeList<int, double, char, float>;
  using Result = typename Slice<0, 4, List>::type;
  static_assert(std::is_same_v<Result, List>,
                "Slice<0, size> should return original list");
}

TEST(SliceTest, MiddleRange) {
  using List = TypeList<int, double, char, float>;
  using Result = typename Slice<1, 3, List>::type;
  static_assert(std::is_same_v<Result, TypeList<double, char>>,
                "Slice<1, 3> should return middle elements");
}

TEST(SliceTest, SingleElement) {
  using List = TypeList<int, double, char, float>;
  using Result = typename Slice<2, 3, List>::type;
  static_assert(std::is_same_v<Result, TypeList<char>>,
                "Slice<2, 3> should return single element");
}

TEST(SliceTest, EmptyRange) {
  using List = TypeList<int, double, char, float>;
  using Result = typename Slice<2, 2, List>::type;
  static_assert(std::is_same_v<Result, TypeList<>>,
                "Slice<2, 2> should return empty list");
}

TEST(SwapTest, SwapAdjacentElements) {
  using List = TypeList<int, double, char, float>;
  using Result = typename Swap<1, 2, List>::type;
  static_assert(std::is_same_v<Result, TypeList<int, char, double, float>>,
                "Should swap adjacent elements");
}

TEST(SwapTest, SwapFirstAndLast) {
  using List = TypeList<int, double, char, float>;
  using Result = typename Swap<0, 3, List>::type;
  static_assert(std::is_same_v<Result, TypeList<float, double, char, int>>,
                "Should swap first and last elements");
}

TEST(SwapTest, SwapMiddleElements) {
  using List = TypeList<int, double, char, float, bool>;
  using Result = typename Swap<1, 3, List>::type;
  static_assert(
      std::is_same_v<Result, TypeList<int, float, char, double, bool>>,
      "Should swap middle elements");
}

TEST(SwapTest, SwapSameIndex) {
  using List = TypeList<int, double, char>;
  using Result = typename Swap<1, 1, List>::type;
  static_assert(std::is_same_v<Result, List>,
                "Swapping same index should return original list");
}

TEST(SwapEdgeCasesTest, SwapFirstTwo) {
  using List = TypeList<int, double, char>;
  using Result = typename Swap<0, 1, List>::type;
  static_assert(std::is_same_v<Result, TypeList<double, int, char>>,
                "Should swap first two elements");
}

TEST(SwapEdgeCasesTest, SwapLastTwo) {
  using List = TypeList<int, double, char>;
  using Result = typename Swap<1, 2, List>::type;
  static_assert(std::is_same_v<Result, TypeList<int, char, double>>,
                "Should swap last two elements");
}

TEST(SwapStressTest, LargeListSwap) {
  using List = TypeList<int, double, char, float, bool, void*, int, double>;
  using Result = typename Swap<2, 5, List>::type;
  static_assert(
      std::is_same_v<
          Result, TypeList<int, double, void*, float, bool, char, int, double>>,
      "Should swap elements in large list");
}

TEST(SwapStressTest, MultipleSwapInvocations) {
  using List = TypeList<int, double, char, float>;
  using AfterFirstSwap = typename Swap<0, 1, List>::type;
  using AfterSecondSwap = typename Swap<1, 2, AfterFirstSwap>::type;
  using AfterThirdSwap = typename Swap<2, 3, AfterSecondSwap>::type;

  static_assert(
      std::is_same_v<AfterFirstSwap, TypeList<double, int, char, float>>,
      "First swap failed");
  static_assert(
      std::is_same_v<AfterSecondSwap, TypeList<double, char, int, float>>,
      "Second swap failed");
  static_assert(
      std::is_same_v<AfterThirdSwap, TypeList<double, char, float, int>>,
      "Third swap failed");
}

template <typename LHS, typename RHS>
struct LessThan {
  static constexpr bool value = LHS::value < RHS::value;
};

template <typename LHS, typename RHS>
struct GreaterThan {
  static constexpr bool value = LHS::value > RHS::value;
};

template <typename LHS, typename RHS>
struct ClosestTo4 {
  static constexpr bool value =
      std::abs(LHS::value - 4) < std::abs(RHS::value - 4);
};

struct Int1 {
  static constexpr int value = 1;
};
struct Int2 {
  static constexpr int value = 2;
};
struct Int3 {
  static constexpr int value = 3;
};
struct Int5 {
  static constexpr int value = 5;
};
struct Int7 {
  static constexpr int value = 7;
};

TEST(PerformPredTest, BasicFunctionality) {
  using List = TypeList<Int2, Int3, Int1, Int5, Int7>;

  constexpr int min = PerformPred<int, LessThan, -1, List>::value;
  static_assert(min == 1, "Should find minimum value 1");

  constexpr int max = PerformPred<int, GreaterThan, -1, List>::value;
  static_assert(max == 7, "Should find maximum value 7");
}

TEST(PerformPredTest, SingleElementList) {
  using SingleList = TypeList<Int5>;

  constexpr int single = PerformPred<int, LessThan, -1, SingleList>::value;
  static_assert(single == 5, "Single element list should return its value");
}

TEST(PerformPredTest, EmptyList) {
  using EmptyList = TypeList<>;

  constexpr int empty = PerformPred<int, LessThan, -1, EmptyList>::value;
  static_assert(empty == -1, "Empty list should return NullVal");
}

TEST(PerformPredTest, AllEqualValues) {
  using EqualList = TypeList<Int2, Int2, Int2>;

  constexpr int result = PerformPred<int, LessThan, -1, EqualList>::value;
  static_assert(result == 2, "All equal values should return that value");
}

TEST(PerformPredTest, ComplexSelection) {
  using ComplexList = TypeList<Int7, Int1, Int5, Int2, Int3>;

  constexpr int closest = PerformPred<int, ClosestTo4, -1, ComplexList>::value;
  static_assert(closest == 5 || closest == 3, "Should find value closest to 4");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}