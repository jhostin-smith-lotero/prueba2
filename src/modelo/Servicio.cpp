#include "Servicio.h"
#include <iostream>

namespace modelo {

int Servicio::rentaBase(int tirada) const {
    int cantidad = 0;
    if (dueno()) {
        const std::vector<Propiedad*>& props = dueno()->propiedades();
        for (std::size_t i = 0; i < props.size(); ++i) {
            const Servicio* s = dynamic_cast<Servicio*>(props[i]);
            if (s) {
                cantidad++;
            }
        }
    }
    int factor = 4;
    if (cantidad >= 2) {
        factor = 10;
    }
    return tirada * factor;
}

void Servicio::alCaer(Juego& juego, Jugador& jugador, int tirada) {
    (void)juego;
    std::cout << "Servicio público " << nombre() << " tirada=" << tirada << "\n";
}

}

