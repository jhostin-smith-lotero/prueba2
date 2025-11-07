#include "Jugador.h"

#include "Propiedad.h"

#include <algorithm>

namespace modelo {

Jugador::Jugador(std::string nombre) : nombre_(std::move(nombre)) {}

void Jugador::mover(int pasos, int limite) {
    int nuevo = (posicion_ + pasos) % limite;
    if (nuevo < 0) {
        nuevo += limite;
    }
    posicion_ = nuevo;
}

void Jugador::moverDirecto(int posicion, int limite) {
    if (posicion < 0) {
        posicion = 0;
    }
    posicion_ = posicion % limite;
}

void Jugador::ajustarDinero(int delta) {
    dinero_ += delta;
}

bool Jugador::pagar(int monto) {
    if (dinero_ < monto) {
        return false;
    }
    dinero_ -= monto;
    return true;
}

void Jugador::cobrar(int monto) {
    dinero_ += monto;
}

void Jugador::encarcelar(int posicionCarcel) {
    posicion_ = posicionCarcel;
    enCarcel_ = true;
    turnosEnCarcel_ = 0;
}

void Jugador::liberarDeCarcel() {
    enCarcel_ = false;
    turnosEnCarcel_ = 0;
}

void Jugador::incrementarTurnoCarcel() {
    ++turnosEnCarcel_;
}

void Jugador::reiniciarTurnoCarcel() {
    turnosEnCarcel_ = 0;
}

void Jugador::recibirCartaSalirCarcel() {
    ++cartasSalirCarcel_;
}

bool Jugador::usarCartaSalirCarcel() {
    if (cartasSalirCarcel_ <= 0) {
        return false;
    }
    --cartasSalirCarcel_;
    liberarDeCarcel();
    return true;
}

void Jugador::agregarPropiedad(Propiedad* propiedad) {
    propiedades_.push_back(propiedad);
}

void Jugador::quitarPropiedad(Propiedad* propiedad) {
    auto it = std::find(propiedades_.begin(), propiedades_.end(), propiedad);
    if (it != propiedades_.end()) {
        propiedades_.erase(it);
    }
}

} // namespace modelo
