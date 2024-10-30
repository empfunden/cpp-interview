#pragma once

#include "types.hpp"
#include <span>

namespace aqtc {

class Planespotter {
private:
public:
  // The proximity threshold determines how close two planes have to be for
  // them to represent a proximity violation.
  //
  // The prediction horizon specifies how far into the future to predict
  // aircraft positions: if the aircraft maintained their linear flight tracks
  // for this duration (in seconds), an alert should be raised if a
  // proximity violation is predicted to occur.
  Planespotter(uint16_t proximity_threshold, uint8_t prediction_horizon);

  // Given a message, return a view of aircraft IDs participating in any
  // predicted proximity violations.
  std::span<uint16_t> handle(const PositionMessage &message);

  void handle(const AnnouncementMessage &message);
};

} // namespace aqtc