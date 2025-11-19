#include "Ferrocarril.h"
#include <iostream>

namespace modelo {

int Ferrocarril::rentaBase(int tirada) const {
    (void)tirada;
    int cantidad = 0;
    if (dueno()) {
        const std::vector<Propiedad*>& props = dueno()->propiedades();
        for (std::size_t i = 0; i < props.size(); ++i) {
            const Ferrocarril* f = dynamic_cast<Ferrocarril*>(props[i]);
            if (f) {
                cantidad++;
            }
        }
    }
    if (cantidad <= 0) {
        return datos().rentaBase();
    }
    int indice = cantidad - 1;
    if (indice > 3) indice = 3;
    return datos().rentaConCasas(indice);
}

void Ferrocarril::alCaer(Juego& juego, Jugador& jugador, int tirada) {
    (void)juego;
    (void)tirada;
    std::cout << "Linea ferroviaria: " << nombre() << "\n";
}

}

