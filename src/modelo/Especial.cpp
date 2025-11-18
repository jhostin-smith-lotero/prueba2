#include "Especial.h"
#include <iostream>

namespace modelo {

Especial::Especial(const std::string& nombre, TipoEspecial tipo)
    : Casilla(nombre, Casilla::Tipo::ESPECIAL), tipoEspecial_(tipo) {}

void Especial::alCaer(Juego& juego, Jugador& jugador, int tirada) {
    (void)juego;
    (void)tirada;
    std::cout << "Casilla especial: " << nombre() << "\n";
    if (tipoEspecial_ == IR_CARCEL) {
        jugador.encarcelar(10);
    }
}

}
