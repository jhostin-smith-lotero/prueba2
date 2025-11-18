#include "Servicio.h"
#include <iostream>

namespace modelo {

int Servicio::rentaBase(int tirada) const {
    int factor = 4;
    return tirada * factor;
}

void Servicio::alCaer(Juego& juego, Jugador& jugador, int tirada) {
    (void)juego;
    std::cout << "Servicio público " << nombre() << " tirada=" << tirada << "\n";
}

}
