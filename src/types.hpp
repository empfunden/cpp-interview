#pragma once

#include <cstdint>

namespace aqtc {

struct [[gnu::packed]] Vec3 {
  int16_t x{};
  int16_t y{};
  int16_t z{};
};
static_assert(sizeof(Vec3) == 6);

struct [[gnu::packed]] PositionMessage {
  uint64_t timestamp_ns{};
  uint16_t aircraft_id{};
  Vec3 position;
  Vec3 direction;
};
static_assert(sizeof(PositionMessage) == 22);

struct [[gnu::packed]] AnnouncementMessage {
  bool entering{};
  uint16_t aircraft_id{};
};
static_assert(sizeof(AnnouncementMessage) == 3);

struct CorrectiveAction {
  uint16_t aircraft_id;
  Vec3 direction;
};

} // namespace aqtc