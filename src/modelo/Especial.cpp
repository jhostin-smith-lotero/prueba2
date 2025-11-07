#include "Especial.h"

#include "Jugador.h"
#include "../juego/Juego.h"

#include <utility>

namespace modelo {

Especial::Especial(std::string nombre, Categoria categoria, int monto)
    : Casilla(std::move(nombre), Tipo::ESPECIAL), categoria_(categoria), monto_(monto) {}

void Especial::alCaer(juego::Juego& juego, Jugador& jugador, int) {
    switch (categoria_) {
        case Categoria::SALIDA:
            juego.cobrarSalida(jugador);
            break;
        case Categoria::ARCA:
            juego.robarCartaArca(jugador);
            break;
        case Categoria::CASUALIDAD:
            juego.robarCartaCasualidad(jugador);
            break;
        case Categoria::IMPUESTO:
        case Categoria::IMPUESTO_LUJO:
            juego.cobrarImpuesto(jugador, monto_);
            break;
        case Categoria::CARCEL:
            juego.visitarCarcel(jugador);
            break;
        case Categoria::IR_CARCEL:
            juego.enviarJugadorCarcel(jugador);
            break;
        case Categoria::ESTACIONAMIENTO:
            juego.estacionamientoLibre(jugador);
            break;
        case Categoria::NADA:
            break;
    }
}

} // namespace modelo
