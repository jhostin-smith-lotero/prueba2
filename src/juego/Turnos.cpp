#include "Turnos.h"

Turnos::Turnos() : numeroTurno(1) {}

void Turnos::siguiente() {
    numeroTurno++;
}

int Turnos::getTurno() const {
    return numeroTurno;
}
