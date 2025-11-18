#include "Construccion.h"
#include <iostream>

bool Construccion::construirCasa(modelo::Propiedad* propiedad, modelo::Jugador* jugador) {
    if (!propiedad || !jugador) return false;
    modelo::Solar* solar = dynamic_cast<modelo::Solar*>(propiedad);
    if (!solar) return false;
    if (solar->dueno() != jugador) return false;
    if (solar->estaHipotecada()) return false;
    if (solar->casas() >= 4 || solar->hotel()) return false;

    int costo = solar->datos().costoCasa();
    if (!jugador->pagar(costo)) return false;
    solar->construirCasa();
    std::cout << jugador->nombre() << " construye una casa en " << solar->nombre() << "\n";
    return true;
}

bool Construccion::construirHotel(modelo::Propiedad* propiedad, modelo::Jugador* jugador) {
    if (!propiedad || !jugador) return false;
    modelo::Solar* solar = dynamic_cast<modelo::Solar*>(propiedad);
    if (!solar) return false;
    if (solar->dueno() != jugador) return false;
    if (solar->estaHipotecada()) return false;
    if (solar->casas() < 4 || solar->hotel()) return false;

    int costo = solar->datos().costoHotel();
    if (!jugador->pagar(costo)) return false;
    solar->construirHotel();
    std::cout << jugador->nombre() << " construye un hotel en " << solar->nombre() << "\n";
    return true;
}
