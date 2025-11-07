#pragma once

#include <string>

namespace juego {
class Banco;
}

namespace modelo {
class Jugador;
class Propiedad;
}

namespace reglas {

class Hipoteca {
public:
    static bool hipotecar(modelo::Propiedad& propiedad,
                           modelo::Jugador& jugador,
                           juego::Banco& banco,
                           int interes,
                           std::string& mensaje);

    static bool deshipotecar(modelo::Propiedad& propiedad,
                              modelo::Jugador& jugador,
                              juego::Banco& banco,
                              int interes,
                              std::string& mensaje);
};

} // namespace reglas
