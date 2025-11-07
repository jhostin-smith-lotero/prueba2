#include "Turnos.h"

namespace juego {

void Turnos::iniciar(std::size_t cantidadJugadores) {
    cantidadJugadores_ = cantidadJugadores;
    actual_ = 0;
    fase_ = Fase::DebeLanzar;
    doblesConsecutivos_ = 0;
}

void Turnos::registrarLanzamiento(bool doble) {
    if (doble) {
        ++doblesConsecutivos_;
        fase_ = Fase::Acciones;
    } else {
        doblesConsecutivos_ = 0;
        fase_ = Fase::Acciones;
    }
}

void Turnos::pasarTurno() {
    if (cantidadJugadores_ == 0) {
        return;
    }
    actual_ = (actual_ + 1) % cantidadJugadores_;
    fase_ = Fase::DebeLanzar;
    doblesConsecutivos_ = 0;
}

void Turnos::forzarTurno(std::size_t jugador) {
    if (jugador < cantidadJugadores_) {
        actual_ = jugador;
        fase_ = Fase::DebeLanzar;
        doblesConsecutivos_ = 0;
    }
}

} // namespace juego
