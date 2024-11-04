#include "Exchange.hpp"

namespace aqtc {

/*
The order book should maintain something like:
Ask 200, 5
Ask 190, 10
            Bid 180, 10
            Bid 180, 10
            Bid 170, 100 &c.
Maintain an ordered map of price level to an ordered map of
id to order quantity for asks. We should use rbegin() on the bid book,
since we want to fetch the highest bid to test against the lowest ask.

*/

// The [[maybe_unused]] in this file are only to prevent warnings
// relating to the fact the implementation is missing.
Exchange::Exchange([[maybe_unused]] TradeReporter &trade_reporter) : trade_reporter_(&trade_reporter) {
  // TODO: Implement
}

void Exchange::on_add([[maybe_unused]] OrderId id, [[maybe_unused]] Side side,
                      [[maybe_unused]] Price price,
                      [[maybe_unused]] Quantity quantity) {
  // TODO: Implement
  if (side == Side::Bid) {
    int_int_map& bids_at_price = bids_[price];
    bids_at_price[id] = quantity;
    id_to_side[id] = Side::Bid;
  }

  else if (side == Side::Ask) {
    int_int_map& asks_at_price = asks_[price];
    asks_at_price[id] = quantity;
    id_to_side[id] = Side::Ask;
  }

  id_to_price[id] = price;

  execute_valid_trades();
}

void Exchange::on_delete([[maybe_unused]] OrderId id) {
  
  Side victim_side = id_to_side[id];
  int victim_price = id_to_price[id];

  if (victim_side == Side::Bid) {
    int_int_map& victim_level = bids_[victim_price];
    victim_level.erase(id);
  }
  else if (victim_side == Side::Ask) {
    int_int_map& victim_level = asks_[victim_price];
    victim_level.erase(id);
  }
}

void Exchange::execute_valid_trades() {
  while (true) {
    if (asks_.empty() || bids_.empty()) {
      break;
    }

    auto& [ask_price, ask_level] = *asks_.begin();
    auto& [bid_price, bid_level] = *bids_.rbegin();

    if (ask_price != bid_price) {
      break;
    }

    auto& [ask_id, ask_qty] = *ask_level.begin();
    auto& [bid_id, bid_qty] = *bid_level.begin();

    Quantity tradeable_qty = std::min(ask_qty, bid_qty);

    // Note that the resting order between the bid/ask is always the one
    // with the lowest id (which must have been placed there first).
    OrderId resting_id = std::min(ask_id, bid_id);
    OrderId matching_id = std::max(ask_id, bid_id);

    ask_qty -= tradeable_qty;
    bid_qty -= tradeable_qty;

    if (ask_qty == 0) {
      ask_level.erase(ask_id);
    }
    if (bid_qty == 0) {
      bid_level.erase(bid_id);
    }

    if (ask_level.empty()) {
      asks_.erase(ask_price);
    }
    if (bid_level.empty()) {
      bids_.erase(bid_price);
    }
  }
}

} // namespace aqtc
