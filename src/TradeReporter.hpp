#pragma once

#include "Types.hpp"

namespace aqtc {

struct TradeReporter {
  TradeReporter() = default;
  virtual ~TradeReporter() = default;
  virtual void on_trade(OrderId order_id, Price price, Quantity quantity) = 0;

  TradeReporter(const TradeReporter &) = delete;
  TradeReporter &operator=(const TradeReporter &) = delete;
};

} // namespace aqtc
