#include "Exchange.hpp"

namespace aqtc {

// The [[maybe_unused]] in this file are only to prevent warnings
// relating to the fact the implementation is missing.
Exchange::Exchange([[maybe_unused]] TradeReporter &trade_reporter) {
  // TODO: Implement
}

void Exchange::on_add([[maybe_unused]] OrderId id, [[maybe_unused]] Side side,
                      [[maybe_unused]] Price price,
                      [[maybe_unused]] Quantity quantity) {
  // TODO: Implement
}

void Exchange::on_delete([[maybe_unused]] OrderId id) {
  // TODO: Implement
}

} // namespace aqtc
