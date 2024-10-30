#include "src/Planespotter.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace aqtc;

// Feel free to modify this file in any way that makes sense to you.

TEST_CASE("A Planespotter") {
  // Configured to alert if any planes are expected to come within 500 meters of
  // each other in the next 30 seconds
  Planespotter spotter{500, 30};

  SECTION("Should allow one plane to fly through") {
    // Entering airspace
    spotter.handle(AnnouncementMessage{true, 1});

    // A few position updates
    CHECK(spotter.handle(PositionMessage{0, 1, {-1, 0, 0}, {1, 0, 0}}).empty());
    CHECK(spotter.handle(PositionMessage{1000000000, 1, {0, 0, 0}, {1, 0, 0}})
              .empty());
    CHECK(spotter.handle(PositionMessage{2000000000, 1, {1, 0, 0}, {1, 0, 0}})
              .empty());

    // Exiting airspace
    spotter.handle(AnnouncementMessage{false, 1});
  }

  SECTION("Should complain if two planes are within 500 meters of each other") {
    spotter.handle(AnnouncementMessage{true, 1});
    spotter.handle(AnnouncementMessage{true, 2});

    CHECK(spotter.handle(PositionMessage{0, 1, {0, 0, 0}, {1, 0, 0}}).empty());

    const auto proximity_violations =
        spotter.handle(PositionMessage{0, 2, {100, 0, 0}, {1, 0, 0}});
    CHECK(!proximity_violations.empty());
  }
}
