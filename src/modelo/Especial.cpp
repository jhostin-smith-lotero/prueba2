#include "Especial.h"
#include "../juego/Juego.h"
#include "../juego/Reglas.h"
#include "../juego/Banco.h"
#include <iostream>

namespace modelo {

Especial::Especial(const std::string& nombre, TipoEspecial tipo, int monto)
    : Casilla(nombre, Casilla::Tipo::ESPECIAL), tipoEspecial_(tipo), monto_(monto) {}

void Especial::alCaer(Juego& juego, Jugador& jugador, int tirada) {
    (void)tirada;
    std::cout << "Casilla especial: " << nombre() << "\n";
    juego.reglas().evaluarCasilla(this, jugador, juego.bancoJugador(), juego.posicionCarcel());
}

}

