#include "Banco.h"

#include "../modelo/Jugador.h"

namespace juego {

void Banco::inicializar(int efectivo, int casasDisponibles, int hotelesDisponibles) {
    efectivo_ = efectivo;
    casasDisponibles_ = casasDisponibles;
    hotelesDisponibles_ = hotelesDisponibles;
}

void Banco::ajustarEfectivo(int delta) {
    efectivo_ += delta;
}

bool Banco::pagarAJugador(modelo::Jugador& jugador, int monto) {
    if (efectivo_ < monto) {
        return false;
    }
    efectivo_ -= monto;
    jugador.cobrar(monto);
    return true;
}

bool Banco::cobrarAJugador(modelo::Jugador& jugador, int monto) {
    if (!jugador.pagar(monto)) {
        return false;
    }
    efectivo_ += monto;
    return true;
}

bool Banco::tomarCasas(int cantidad) {
    if (casasDisponibles_ < cantidad) {
        return false;
    }
    casasDisponibles_ -= cantidad;
    return true;
}

void Banco::devolverCasas(int cantidad) {
    casasDisponibles_ += cantidad;
}

bool Banco::tomarHoteles(int cantidad) {
    if (hotelesDisponibles_ < cantidad) {
        return false;
    }
    hotelesDisponibles_ -= cantidad;
    return true;
}

void Banco::devolverHoteles(int cantidad) {
    hotelesDisponibles_ += cantidad;
}

} // namespace juego
