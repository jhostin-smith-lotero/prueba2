#pragma once

#include <string>

namespace juego {
class Banco;
}

namespace modelo {
class Jugador;
class Solar;
}

namespace reglas {

class Construccion {
public:
    static bool construirCasa(modelo::Solar& solar,
                               modelo::Jugador& jugador,
                               juego::Banco& banco,
                               std::string& mensaje);

    static bool construirHotel(modelo::Solar& solar,
                                modelo::Jugador& jugador,
                                juego::Banco& banco,
                                std::string& mensaje);
};

} // namespace reglas
