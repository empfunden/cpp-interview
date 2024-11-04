#pragma once

#include <map>

namespace aqtc {

using OrderId = unsigned long;
using Price = int;
using Quantity = unsigned int;

using int_int_map = std::map<int, int>;

enum class Side { Bid, Ask };
    
} // namespace aqtc
