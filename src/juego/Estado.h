#pragma once

#include "Banco.h"
#include "Turnos.h"
#include "../modelo/Mazo.h"
#include "../modelo/Jugador.h"

#include <vector>

namespace juego {

class Estado {
public:
    Estado() = default;

    Estado(std::vector<modelo::Jugador> jugadores,
           Banco banco,
           Turnos turnos,
           modelo::Mazo mazoCasualidad,
           modelo::Mazo mazoArca)
        : jugadores_(std::move(jugadores)),
          banco_(std::move(banco)),
          turnos_(std::move(turnos)),
          mazoCasualidad_(std::move(mazoCasualidad)),
          mazoArca_(std::move(mazoArca)) {}

    const std::vector<modelo::Jugador>& jugadores() const { return jugadores_; }
    const Banco& banco() const { return banco_; }
    const Turnos& turnos() const { return turnos_; }
    const modelo::Mazo& mazoCasualidad() const { return mazoCasualidad_; }
    const modelo::Mazo& mazoArca() const { return mazoArca_; }

    std::vector<modelo::Jugador>& jugadores() { return jugadores_; }
    Banco& banco() { return banco_; }
    Turnos& turnos() { return turnos_; }
    modelo::Mazo& mazoCasualidad() { return mazoCasualidad_; }
    modelo::Mazo& mazoArca() { return mazoArca_; }

private:
    std::vector<modelo::Jugador> jugadores_;
    Banco banco_;
    Turnos turnos_;
    modelo::Mazo mazoCasualidad_;
    modelo::Mazo mazoArca_;
};

} // namespace juego
