#pragma once

#include "TypeList.hpp"

namespace type_list::sort {

template <template <typename LHS, typename RHS> typename Pred, typename TList>
struct BubbleSort {
 private:
  static constexpr std::size_t min_ind =
      PerformBinaryPredByIndex<Pred, TList>::value;
  using swapped_type = SwapT<0, min_ind, TList>;

 public:
  using type = AppendFrontT<HeadT<swapped_type>,
                       typename BubbleSort<Pred, TailT<swapped_type>>::type>;
};

template <template <typename LHS, typename RHS> typename Pred, typename T>
struct BubbleSort<Pred, TypeList<T>> {
  using type = TypeList<T>;
};

template <template <typename LHS, typename RHS> typename Pred>
struct BubbleSort<Pred, TypeList<>> {
  using type = TypeList<>;
};

template <template <typename LHS, typename RHS> typename Pred, typename TList>
using BubbleSortT = typename BubbleSort<Pred, TList>::type;

}  // namespace type_list::sort
