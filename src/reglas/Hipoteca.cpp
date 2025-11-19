#include "Hipoteca.h"
#include <iostream>

bool Hipoteca::hipotecar(modelo::Propiedad* propiedad, modelo::Jugador* jugador) {
    if (!propiedad || !jugador) return false;
    if (propiedad->estaHipotecada()) return false;
    if (propiedad->dueno() != jugador) return false;

    int monto = propiedad->datos().precio() / 2;
    jugador->cobrar(monto);
    propiedad->hipotecar();
    std::cout << jugador->nombre() << " hipoteco " << propiedad->nombre() << " por $" << monto << "\n";
    return true;
}

bool Hipoteca::cancelarHipoteca(modelo::Propiedad* propiedad, modelo::Jugador* jugador) {
    if (!propiedad || !jugador) return false;
    if (!propiedad->estaHipotecada()) return false;
    if (propiedad->dueno() != jugador) return false;

    int deuda = propiedad->datos().precio() / 2;
    int interes = deuda / 10;
    int total = deuda + interes;
    if (!jugador->pagar(total)) {
        return false;
    }
    propiedad->deshipotecar();
    std::cout << jugador->nombre() << " cancelo hipoteca pagando $" << total << "\n";
    return true;
}
