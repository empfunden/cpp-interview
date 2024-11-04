#pragma once

#include "TradeReporter.hpp"
#include "Types.hpp"

#include <iostream>
#include <map>
#include <algorithm>

namespace aqtc {

class Exchange {
public:
  explicit Exchange(TradeReporter &reporter);
  void on_add(OrderId id, Side side, Price price, Quantity quantity);
  void on_delete(OrderId id);

private:
  TradeReporter* trade_reporter_;
  std::map<int, int_int_map> bids_;
  std::map<int, int_int_map> asks_;
  std::map<int, Side> id_to_side;
  std::map<int, int> id_to_price;

  void execute_valid_trades();
};

} // namespace aqtc
