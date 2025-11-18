#include "Reglas.h"
#include <iostream>

void Reglas::evaluarCasilla(modelo::Jugador& jugador, Banco& banco, Estado& estado) {
    (void)estado;
    int pos = jugador.posicion();

    if (pos == 0) {
        std::cout << jugador.nombre() << " recibe $200 por pasar por GO.\n";
        banco.recibirDelBanco(jugador.nombre(), 200);
        jugador.cobrar(200);
    }
    else if (pos == 10) {
        std::cout << jugador.nombre() << " es enviado a la cárcel.\n";
        jugador.encarcelar(10);
    }
    else if (pos == 30) {
        std::cout << jugador.nombre() << " paga una multa de $100.\n";
        banco.pagarBanco(jugador.nombre(), 100);
        jugador.pagar(100);
    }
    else {
        std::cout << jugador.nombre() << " cae en una casilla sin evento.\n";
    }
}
