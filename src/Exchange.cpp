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
Exchange::Exchange(TradeReporter &trade_reporter) : 
  trade_reporter_ptr_(&trade_reporter) { }

void Exchange::on_add(OrderId id, Side side,
                      Price price,
                      Quantity quantity) {
  
  // We add the order to the correct side and then
  // call a helper to execute all trades that can now occur.

  if (side == Side::Bid) {
    int_int_map& bids_at_price = bids_[price];
    bids_at_price[id] = quantity;
  }

  else if (side == Side::Ask) {
    int_int_map& asks_at_price = asks_[price];
    asks_at_price[id] = quantity;
  }

  id_to_side[id] = side;
  id_to_price[id] = price;

  execute_valid_trades();
}

void Exchange::on_delete( OrderId id) {
  
  Side victim_side = id_to_side[id];
  int victim_price = id_to_price[id];

  // id_to_... are bookkeeping maps for deletion purposes, so
  // we can get delete the data since we'll not need it again.

  id_to_side.erase(id);
  id_to_price.erase(id);

  // Delete the order, as well as the corresponding 
  // price level if it's now empty.

  if (victim_side == Side::Bid) {
    int_int_map& victim_level = bids_[victim_price];
    victim_level.erase(id);

    if (victim_level.empty()) {
      bids_.erase(victim_price);
    }
  }
  
  else if (victim_side == Side::Ask) {
    int_int_map& victim_level = asks_[victim_price];
    victim_level.erase(id);

    if (victim_level.empty()) {
      asks_.erase(victim_price);
    }
  }
}

void Exchange::execute_valid_trades() {
  while (true) {

    // We can keep going until either bids/asks are exhausted or
    // there's no more good price overlap (see below).
    if (asks_.empty() || bids_.empty()) {
      break;
    }

    // We want to see if there's any price overlap b/w the
    // min ask (best seller) and max bid (best buyer).
    auto& [ask_price, ask_level] = *asks_.begin();
    auto& [bid_price, bid_level] = *bids_.rbegin();

    if (ask_price > bid_price) {
      break;
    }

    auto& [ask_id, ask_qty] = *ask_level.begin();
    auto& [bid_id, bid_qty] = *bid_level.begin();

    Quantity tradeable_qty = std::min(ask_qty, bid_qty);
    Price resting_price;  // Not necessary; extra var for readability.

    // Note that the resting order between the bid/ask must be the one
    // with the lowest id (which must have been placed there first),
    // and order matters here for recording trades (resting goes first)
    // So we just condition the order of recorded trades on that.
    // We could also do this using minmax and make_pair (more complicated).

    if (ask_id < bid_id) {
      resting_price = ask_price;
      trade_reporter_ptr_->on_trade(ask_id, resting_price, tradeable_qty);
      trade_reporter_ptr_->on_trade(bid_id, resting_price, tradeable_qty);
    }
    else {
      resting_price = bid_price;
      trade_reporter_ptr_->on_trade(bid_id, resting_price, tradeable_qty);
      trade_reporter_ptr_->on_trade(ask_id, resting_price, tradeable_qty);
    }

    // Handle the mutual elimination of valid trades, and remove empty
    // remaining orders and bids where necessary. 

    ask_qty -= tradeable_qty;
    bid_qty -= tradeable_qty;

    if (ask_qty <= 0) {
      ask_level.erase(ask_id);
      id_to_side.erase(ask_id);
      id_to_price.erase(ask_id);
    }
    if (bid_qty <= 0) {
      bid_level.erase(bid_id);
      id_to_side.erase(bid_id);
      id_to_price.erase(bid_id);
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
