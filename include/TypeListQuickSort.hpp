#pragma once

#include "TypeList.hpp"

namespace type_list::sort {

template <typename TList, template<typename LHS, typename RHS> typename Pred>
struct QuickSort {};

}  // namespace type_list::sort
