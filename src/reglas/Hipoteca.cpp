#include "Hipoteca.h"
#include <iostream>
#include "../modelo/Propiedad.h"
#include "../modelo/Jugador.h"

/*
 SUPOSICIONES:
  - Propiedad:
    - bool isHipotecada() const;
    - void setHipotecada(bool);
    - int getPrecio() const;
    - std::string getPropietarioName() const;
  - Jugador:
    - void recibir(int monto);
    - bool pagar(int monto);
    - std::string getNombre() const;
*/

bool Hipoteca::hipotecar(Propiedad* propiedad, Jugador* jugador) {
    if (!propiedad || !jugador) return false;
    if (propiedad->isHipotecada()) return false;
    if (propiedad->getPropietarioName() != jugador->getNombre()) return false;

    int monto = propiedad->getPrecio() / 2; // ejemplo: 50% del precio
    jugador->recibir(monto);
    propiedad->setHipotecada(true);
    std::cout << jugador->getNombre() << " hipotecó " << propiedad->getNombre() << " por $" << monto << ".\n";
    return true;
}

bool Hipoteca::cancelarHipoteca(Propiedad* propiedad, Jugador* jugador) {
    if (!propiedad || !jugador) return false;
    if (!propiedad->isHipotecada()) return false;
    if (propiedad->getPropietarioName() != jugador->getNombre()) return false;

    // interes ejemplo 10%
    int deuda = (propiedad->getPrecio() / 2);
    int interes = deuda / 10;
    int total = deuda + interes;

    if (!jugador->pagar(total)) return false;
    propiedad->setHipotecada(false);
    std::cout << jugador->getNombre() << " canceló la hipoteca de " << propiedad->getNombre() << " pagando $" << total << ".\n";
    return true;
}
