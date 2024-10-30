#include "src/Planespotter.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <fmt/ranges.h>

using namespace aqtc;

namespace {

void check_id_in(uint16_t aircraft_id, std::span<uint16_t> aircraft_ids) {
  for (const auto id : aircraft_ids) {
    if (id == aircraft_id)
      return;
  }
  FAIL(fmt::format("Couldn't find {} in: {}", aircraft_id,
                   fmt::join(aircraft_ids, ",")));
}

} // namespace

// Feel free to modify this file in any way that makes sense to you.

TEST_CASE("A Planespotter") {
  // Configured to alert if any planes are expected to come within 500 meters of
  // each other in the next 30 seconds
  Planespotter spotter{500, 30};

  SECTION("Should allow one plane to fly through") {
    // Entering airspace
    spotter.handle(AnnouncementMessage{true, 1});

    // A few position updates
    CHECK(spotter.handle(PositionMessage{0, 1, {-1, 0, 0}, {1, 0, 0}, 0})
              .empty());
    CHECK(
        spotter.handle(PositionMessage{1000000000, 1, {0, 0, 0}, {1, 0, 0}, 0})
            .empty());
    CHECK(
        spotter.handle(PositionMessage{2000000000, 1, {1, 0, 0}, {1, 0, 0}, 0})
            .empty());

    // Exiting airspace
    spotter.handle(AnnouncementMessage{false, 1});
  }

  SECTION("Should complain if two planes are reported within 500 meters of "
          "each other at the same time") {
    spotter.handle(AnnouncementMessage{true, 1});
    spotter.handle(AnnouncementMessage{true, 2});

    CHECK(
        spotter.handle(PositionMessage{0, 1, {0, 0, 0}, {1, 0, 0}, 0}).empty());

    const auto proximity_violations =
        spotter.handle(PositionMessage{0, 2, {100, 0, 0}, {1, 0, 0}, 0});
    check_id_in(1, proximity_violations);
    check_id_in(2, proximity_violations);
  }

  SECTION("Should complain if two planes are projected to be within 500 meters "
          "of each other in the next 30 seconds") {
    spotter.handle(AnnouncementMessage{true, 1});
    spotter.handle(AnnouncementMessage{true, 2});

    // Plane 1 moving at 250m/s along the x-axis will be 7500m away in 30s
    CHECK(spotter.handle(PositionMessage{0, 1, {0, 0, 0}, {1, 0, 0}, 250})
              .empty());

    // Plane 2 for simplicity has a speed of 0, hovering at x=7900
    const auto proximity_violations =
        spotter.handle(PositionMessage{0, 2, {7900, 0, 0}, {1, 0, 0}, 0});
    check_id_in(1, proximity_violations);
    check_id_in(2, proximity_violations);
  }
}
