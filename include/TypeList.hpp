#pragma once

#include <cstddef>

namespace type_list {

template <typename... Ts>
struct TypeList {};

template <typename... Ts>
struct Head;

template <typename... Ts>
struct Tail;

template <typename T, typename... Ts>
struct Head<TypeList<T, Ts...>> {
  using type = T;
};

template <typename T, typename... Ts>
struct Tail<TypeList<T, Ts...>> {
  using type = TypeList<Ts...>;
};

template <std::size_t I, typename TList>
struct Get {
  using type = Get<I - 1, typename Tail<TList>::type>::type;
};

template <typename TList>
struct Get<0, TList> {
  using type = typename Head<TList>::type;
};

}  // namespace type_list
