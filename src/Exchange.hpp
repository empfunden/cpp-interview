#pragma once

#include "TradeReporter.hpp"
#include "Types.hpp"

namespace aqtc {

class Exchange {
public:
  explicit Exchange(TradeReporter &reporter);
  void on_add(OrderId id, Side side, Price price, Quantity quantity);
  void on_delete(OrderId id);
};

} // namespace aqtc
