#include "Solar.h"
#include <iostream>

namespace modelo {

Solar::Solar(const std::string& nombre, const Escritura& escritura, const std::string& color)
    : Propiedad(nombre, escritura, Casilla::Tipo::SOLAR), color_(color), casas_(0), hotel_(false) {}

void Solar::construirCasa() {
    if (casas_ < 4 && !hotel_) {
        ++casas_;
    }
}

void Solar::construirHotel() {
    if (casas_ == 4 && !hotel_) {
        hotel_ = true;
        casas_ = 0;
    }
}

void Solar::fijarEdificaciones(int casas, bool hotel) {
    casas_ = casas;
    hotel_ = hotel;
}

int Solar::rentaBase(int tirada) const {
    (void)tirada;
    if (hotel_) {
        return escritura_.rentaConHotel();
    }
    return escritura_.rentaConCasas(casas_);
}

void Solar::alCaer(Juego& juego, Jugador& jugador, int tirada) {
    (void)juego;
    (void)tirada;
    std::cout << "Estas en " << nombre() << " (grupo " << color_ << ")\n";
}

}
