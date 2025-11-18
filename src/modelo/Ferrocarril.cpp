#include "Ferrocarril.h"
#include <iostream>

namespace modelo {

int Ferrocarril::rentaBase(int tirada) const {
    (void)tirada;
    return datos().rentaBase();
}

void Ferrocarril::alCaer(Juego& juego, Jugador& jugador, int tirada) {
    (void)juego;
    (void)tirada;
    std::cout << "Línea ferroviaria: " << nombre() << "\n";
}

}
