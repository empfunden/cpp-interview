#include "src/Exchange.hpp"
#include "src/TradeReporter.hpp"
#include "src/Types.hpp"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

using namespace aqtc;
using Side::Ask;
using Side::Bid;

namespace {

struct RecordedTrade {
  OrderId id;
  Price price;
  Quantity quantity;

  bool operator==(const RecordedTrade &) const = default;
};

std::ostream &operator<<(std::ostream &os, const RecordedTrade &value) {
  os << "{ ID: " << value.id << ", Price: " << value.price
     << ", Quantity: " << value.quantity << "}";
  return os;
}

struct TradeRecorder : public TradeReporter {
  void on_trade(OrderId order_id, Price price, Quantity quantity) override {
    record_.emplace_back(order_id, price, quantity);
  }

  const auto &record() const { return record_; }

private:
  std::vector<RecordedTrade> record_;
};

} // namespace

// Feel free to modify this file in any way that makes sense to you.

TEST_CASE("An Exchange") {
  TradeRecorder recorder;
  Exchange exchange{recorder};

  SECTION("can match orders at the same price level and quantity") {
    exchange.on_add(1, Bid, 100, 50);
    exchange.on_add(2, Ask, 100, 50);
    exchange.on_add(3, Ask, 100, 50);

    // Both the resting and matching order should be reported. Order
    // 3 should be ignored, as order 1 will have been used up completely
    // by order 2. The resting order should be reported first
    CHECK(recorder.record() ==
          std::vector<RecordedTrade>{RecordedTrade{1, 100, 50},
                                     RecordedTrade{2, 100, 50}});
  }

  SECTION("can match an order against multiple resting orders") {
    exchange.on_add(1, Bid, 33, 25);
    exchange.on_add(2, Bid, 36, 11);
    exchange.on_add(3, Ask, 31, 30);

    // The incoming order is reported once per (possibly partial) match
    CHECK(recorder.record() ==
          std::vector<RecordedTrade>{
              RecordedTrade{2, 36, 11}, RecordedTrade{3, 36, 11},
              RecordedTrade{1, 33, 19}, RecordedTrade{3, 33, 19}});
  }

  SECTION("can delete an order") {
    exchange.on_add(1, Bid, 100, 50);
    exchange.on_delete(1);
    exchange.on_add(2, Ask, 100, 50);

    CHECK(recorder.record() == std::vector<RecordedTrade>{});
  }
}
