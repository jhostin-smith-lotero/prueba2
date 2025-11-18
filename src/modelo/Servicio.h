#ifndef SERVICIO_H
#define SERVICIO_H

#include "Propiedad.h"

namespace modelo {

struct Servicio : public Propiedad {
public:
    Servicio(const std::string& nombre, const Escritura& escritura)
        : Propiedad(nombre, escritura, Casilla::Tipo::SERVICIO) {}

    int rentaBase(int tirada) const override;
    void alCaer(Juego& juego, Jugador& jugador, int tirada) override;
};

}

#endif //SERVICIO_H
