#include "Dado.h"

#include <chrono>

namespace juego {

Dado::Dado() : rng_(static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count())), dist_(1, 6) {}

std::pair<int, int> Dado::lanzar() {
    return {dist_(rng_), dist_(rng_)};
}

} // namespace juego
