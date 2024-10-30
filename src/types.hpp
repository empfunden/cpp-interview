#pragma once

#include <cstdint>

namespace aqtc {

struct Vec3 {
  int16_t x{};
  int16_t y{};
  int16_t z{};
};

struct PositionMessage {
  uint32_t timestamp{};
  uint16_t aircraft_id{};
  Vec3 position;
  Vec3 direction;
  uint16_t speed{};
};

struct AnnouncementMessage {
  bool entering{};
  uint16_t aircraft_id{};
};

} // namespace aqtc