#include "Propiedad.h"
#include <iostream>

namespace modelo {

void Propiedad::alCaer(Juego& juego, Jugador& jugador, int tirada) {
    (void)juego;
    (void)tirada;
    std::cout << jugador.nombre() << " llegó a " << nombre() << "\n";
}

}
