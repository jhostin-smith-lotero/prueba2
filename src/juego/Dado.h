#pragma once

#include <random>
#include <utility>

namespace juego {

class Dado {
public:
    Dado();
    std::pair<int, int> lanzar();

private:
    std::mt19937 rng_;
    std::uniform_int_distribution<int> dist_;
};

} // namespace juego
