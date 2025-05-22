#pragma once

#include <cstddef>

namespace type_list {

template <typename... Ts>
struct TypeList {};

template <typename TList1, typename TList2>
struct IsSameList : std::false_type {};

template <typename... Ts>
struct IsSameList<TypeList<Ts...>, TypeList<Ts...>> : std::true_type {};

template <typename TList1, typename TList2>
constexpr bool IsSameListV = IsSameList<TList1, TList2>::value;

template <typename... Ts>
struct Head;

template <typename T, typename... Ts>
struct Head<TypeList<T, Ts...>> {
  using type = T;
};

template <typename TList>
using HeadT = typename Head<TList>::type;

template <typename... Ts>
struct Tail;

template <typename T, typename... Ts>
struct Tail<TypeList<T, Ts...>> {
  using type = TypeList<Ts...>;
};

template <typename TList>
using TailT = typename Tail<TList>::type;

template <std::size_t I, typename TList>
struct Get {
  using type = Get<I - 1, TailT<TList>>::type;
};

template <typename TList>
struct Get<0, TList> {
  using type = HeadT<TList>;
};

template <std::size_t I, typename TList>
using GetT = typename Get<I, TList>::type;

template <typename TList>
struct Size;

template <typename... Ts>
struct Size<TypeList<Ts...>> {
  static constexpr std::size_t value = sizeof...(Ts);
};

template <typename TList>
constexpr std::size_t SizeV = Size<TList>::value;

template <typename TList1, typename TList2>
struct Concat;

template <typename... Ts, typename... Us>
struct Concat<TypeList<Ts...>, TypeList<Us...>> {
  using type = TypeList<Ts..., Us...>;
};

template <typename TList1, typename TList2>
using ConcatT = typename Concat<TList1, TList2>::type;

template <typename T, typename TList>
struct AppendBack;

template <typename T, typename... Ts>
struct AppendBack<T, TypeList<Ts...>> {
  using type = TypeList<Ts..., T>;
};

template <typename T, typename TList>
using AppendBackT = typename AppendBack<T, TList>::type;

template <typename T, typename TList>
struct AppendFront;

template <typename T, typename... Ts>
struct AppendFront<T, TypeList<Ts...>> {
  using type = TypeList<T, Ts...>;
};

template <typename T, typename TList>
using AppendFrontT = typename AppendFront<T, TList>::type;

template <typename TList>
struct PopBack;

template <typename T, typename... Ts>
struct PopBack<TypeList<T, Ts...>> {
  using type = ConcatT<TypeList<T>, typename PopBack<TypeList<Ts...>>::type>;
};

template <typename T>
struct PopBack<TypeList<T>> {
  using type = TypeList<>;
};

template <typename TList>
using PopBackT = typename PopBack<TList>::type;

template <typename TList>
struct PopFront;

template <typename T, typename... Ts>
struct PopFront<TypeList<T, Ts...>> {
  using type = TypeList<Ts...>;
};

template <typename TList>
using PopFrontT = typename PopFront<TList>::type;

template <std::size_t I, typename TList>
struct DropFront {
  using type = typename DropFront<I - 1, typename PopFront<TList>::type>::type;
};

template <typename TList>
struct DropFront<0, TList> {
  using type = TList;
};

template <std::size_t I, typename TList>
using DropFrontT = typename DropFront<I, TList>::type;

template <std::size_t I, typename TList>
struct DropBack {
  using type = typename DropBack<I - 1, typename PopBack<TList>::type>::type;
};

template <typename TList>
struct DropBack<0, TList> {
  using type = TList;
};

template <std::size_t I, typename TList>
using DropBackT = typename DropBack<I, TList>::type;

template <std::size_t L, std::size_t R, typename TList>
struct Slice {
  using type = DropBackT<Size<TList>::value - R, DropFrontT<L, TList>>;
};

template <std::size_t L, std::size_t R, typename TList>
using SliceT = typename Slice<L, R, TList>::type;

template <std::size_t I, std::size_t J, typename TList>
struct Swap {
  // clang-format off
  using type = 
    ConcatT<
      ConcatT<
        ConcatT<
          ConcatT<
            SliceT<0, I, TList>,
            TypeList<
              GetT<J, TList>
            >
          >,
          SliceT<I + 1, J, TList>
        >,
        TypeList<
          GetT<I, TList>
        >
      >,
      SliceT<J + 1, SizeV<TList>, TList>
    >;
  // clang-format on
};

template <std::size_t I, typename TList>
struct Swap<I, I, TList> {
  using type = TList;
};

template <std::size_t I, std::size_t J, typename TList>
using SwapT = typename Swap<I, J, TList>::type;

template <std::size_t I, typename TList>
struct Erase {
  using type = ConcatT<SliceT<0, I, TList>, SliceT<I + 1, SizeV<TList>, TList>>;
};

template <std::size_t I, typename TList>
using EraseT = typename Erase<I, TList>::type;

template <typename ValueType,
          template <typename LHS, typename RHS> typename Pred,
          ValueType NullVal, typename TList>
struct PerformBinaryPredByValue;

template <typename ValueType,
          template <typename LHS, typename RHS> typename Pred,
          ValueType NullVal, typename T, typename U, typename... Ts>
struct PerformBinaryPredByValue<ValueType, Pred, NullVal,
                                TypeList<T, U, Ts...>> {
  static constexpr ValueType value =
      Pred<T, U>::value ? PerformBinaryPredByValue<ValueType, Pred, NullVal,
                                                   TypeList<T, Ts...>>::value
                        : PerformBinaryPredByValue<ValueType, Pred, NullVal,
                                                   TypeList<U, Ts...>>::value;
};

template <typename ValueType,
          template <typename LHS, typename RHS> typename Pred,
          ValueType NullVal, typename T>
struct PerformBinaryPredByValue<ValueType, Pred, NullVal, TypeList<T>> {
  static constexpr ValueType value = T::value;
};

template <typename ValueType,
          template <typename LHS, typename RHS> typename Pred,
          ValueType NullVal>
struct PerformBinaryPredByValue<ValueType, Pred, NullVal, TypeList<>> {
  static constexpr ValueType value = NullVal;
};

template <template <typename LHS, typename RHS> typename Pred, std::size_t TI,
          std::size_t UI, typename TList>
struct PerformBinaryPredByIndexImpl;

template <template <typename LHS, typename RHS> typename Pred, std::size_t TI,
          std::size_t UI, typename T, typename U, typename... Ts>
struct PerformBinaryPredByIndexImpl<Pred, TI, UI, TypeList<T, U, Ts...>> {
 private:
  static constexpr bool pred_result = Pred<T, U>::value;
  using NextT = std::conditional_t<pred_result, T, U>;
  static constexpr std::size_t NextV = pred_result ? TI : UI;

 public:
  static constexpr std::size_t value =
      PerformBinaryPredByIndexImpl<Pred, NextV, UI + 1,
                                   TypeList<NextT, Ts...>>::value;
};

template <template <typename LHS, typename RHS> typename Pred, std::size_t TI,
          std::size_t UI, typename T, typename U>
struct PerformBinaryPredByIndexImpl<Pred, TI, UI, TypeList<T, U>> {
  static constexpr std::size_t value = Pred<T, U>::value ? TI : UI;
};

template <template <typename LHS, typename RHS> typename Pred, typename TList>
struct PerformBinaryPredByIndex {
  static constexpr std::size_t value = []() {
    if constexpr (SizeV<TList> < 2ull) {
      return SizeV<TList> - 1ull;
    } else {
      return PerformBinaryPredByIndexImpl<Pred, 0, 1, TList>::value;
    }
  }();
};

}  // namespace type_list
