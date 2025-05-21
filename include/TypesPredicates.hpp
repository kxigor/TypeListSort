#pragma once

#include <algorithm>
#include <ranges>
#include <string_view>

#include "TypeList.hpp"

namespace type_list::predicates {
template <typename LHS, typename RHS>
struct SizeLessPred {
  static constexpr bool value = sizeof(LHS) < sizeof(RHS);
};

template <typename LHS, typename RHS>
struct SizeGreaterPred {
  static constexpr bool value = sizeof(RHS) < sizeof(LHS);
};

template <typename LHS, typename RHS>
struct AlignLessPred {
  static constexpr bool value = alignof(LHS) < alignof(RHS);
};

template <typename LHS, typename RHS>
struct AlignGreaterPred {
  static constexpr bool value = alignof(LHS) > alignof(RHS);
};

template <typename LHS, typename RHS>
struct IsBaseOfPred {
  static constexpr bool value = std::is_base_of_v<LHS, RHS>;
};

template <typename LHS, typename RHS>
struct IsConstructiblePred {
  static constexpr bool value = std::is_constructible_v<RHS, LHS>;
};

template <typename LHS, typename RHS>
struct ConvertsBetterPred {
  static constexpr bool value = std::convertible_to<LHS, RHS> &&
                                requires(LHS from) { static_cast<RHS>(from); };
};

template <typename LHS, typename RHS>
struct IsMoreDerivedPred {
  static constexpr bool value =
      std::is_base_of_v<RHS, LHS> && !std::is_same_v<LHS, RHS>;
};

namespace details {
template <typename T>
constexpr auto type_name() {
#if defined(__clang__) || defined(__GNUC__)
  constexpr std::string_view prefix = "T = ";
  constexpr std::string_view func = __PRETTY_FUNCTION__;
  constexpr size_t start = func.find(prefix) + prefix.size();
  constexpr size_t end = func.find_last_of(']');
  constexpr std::string_view name = func.substr(start, end - start);
  return name;
#elif defined(_MSC_VER)
  constexpr std::string_view func = __FUNCSIG__;
  constexpr std::string_view prefix = "type_name<";
  constexpr std::string_view suffix = ">(void)";
  constexpr size_t start = func.find(prefix) + prefix.size();
  constexpr size_t end = func.rfind(suffix);
  constexpr std::string_view name = func.substr(start, end - start);
  constexpr std::string_view struct_prefix = "struct ";
  constexpr std::string_view class_prefix = "class ";
  constexpr std::string_view enum_prefix = "enum ";
  constexpr size_t clean_start = [&]() {
    if (name.substr(0, struct_prefix.size()) == struct_prefix)
      return struct_prefix.size();
    if (name.substr(0, class_prefix.size()) == class_prefix)
      return class_prefix.size();
    if (name.substr(0, enum_prefix.size()) == enum_prefix)
      return enum_prefix.size();
    return size_t(0);
  }();
  return name.substr(clean_start);
#endif
}
}  // namespace details

template <typename LHS, typename RHS>
struct TypeNameLessPred {
  static constexpr bool value = std::ranges::lexicographical_compare(
      details::type_name<LHS>(), details::type_name<RHS>());
};

template <typename LHS, typename RHS>
struct TypeNameGreaterPred {
  static constexpr bool value = std::ranges::lexicographical_compare(
      details::type_name<RHS>(), details::type_name<LHS>());
};

namespace details {
template <typename T>
struct GetBasesTypeList {
#ifdef __GNUC__
  using type = TypeList<__direct_bases(T)...>;
#endif
};

template <typename T>
using GetBasesTypeListT = typename GetBasesTypeList<T>::type;

template <typename TList>
struct GetDepthOfInheritanceImpl;

template <typename... Ts>
struct GetDepthOfInheritanceImpl<TypeList<Ts...>> {
  static constexpr std::size_t value =
      std::max({GetDepthOfInheritanceImpl<GetBasesTypeListT<Ts>>::value...}) +
      1ull;
};

template <>
struct GetDepthOfInheritanceImpl<TypeList<>> {
  static constexpr std::size_t value = 0;
};

template <typename T>
struct GetDepthOfInheritance {
  static constexpr std::size_t value =
      GetDepthOfInheritanceImpl<GetBasesTypeListT<T>>::value;
};

template <typename T>
static constexpr std::size_t GetDepthOfInheritanceV =
    GetDepthOfInheritance<T>::value;

};  // namespace details

template <typename LHS, typename RHS>
struct DepthOfInheritanceLessPred {
  static constexpr bool value = details::GetDepthOfInheritanceV<LHS> <
                                details::GetDepthOfInheritanceV<RHS>;
};

template <typename LHS, typename RHS>
struct DepthOfInheritanceGreaterPred {
  static constexpr bool value = details::GetDepthOfInheritanceV<RHS> <
                                details::GetDepthOfInheritanceV<LHS>;
};

};  // namespace type_list::predicates
