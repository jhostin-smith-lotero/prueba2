#include "Propiedad.h"

#include "Jugador.h"
#include "../juego/Juego.h"

namespace modelo {

Propiedad::Propiedad(std::string nombre, Tipo tipo, Escritura escritura)
    : Casilla(std::move(nombre), tipo), escritura_(escritura) {}

void Propiedad::asignarDueno(Jugador* jugador) {
    dueno_ = jugador;
}

void Propiedad::liberar() {
    dueno_ = nullptr;
    hipotecada_ = false;
}

void Propiedad::alCaer(juego::Juego& juego, Jugador& jugador, int tirada) {
    juego.resolverPropiedad(*this, jugador, tirada);
}

} // namespace modelo
