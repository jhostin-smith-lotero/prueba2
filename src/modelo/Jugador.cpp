#include "Jugador.h"
#include "Propiedad.h"

namespace modelo {

Jugador::Jugador(const std::string& nombre) : nombre_(nombre) {}

void Jugador::mover(int pasos, int limite) {
    if (limite <= 0) {
        return;
    }
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
    for (std::vector<Propiedad*>::iterator it = propiedades_.begin(); it != propiedades_.end(); ++it) {
        if (*it == propiedad) {
            propiedades_.erase(it);
            break;
        }
    }
}

void Jugador::limpiarPropiedades() {
    propiedades_.clear();
}

}
