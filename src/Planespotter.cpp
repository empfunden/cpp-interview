#include "Planespotter.hpp"
#include "types.hpp"

namespace aqtc {

Planespotter::Planespotter([[maybe_unused]] uint16_t proximity_threshold,
                           [[maybe_unused]] uint8_t prediction_horizon) {}

std::span<uint16_t>
Planespotter::handle([[maybe_unused]] const PositionMessage &message) {
  return {};
}

void Planespotter::handle([[maybe_unused]] const AnnouncementMessage &message) {

}

} // namespace aqtc