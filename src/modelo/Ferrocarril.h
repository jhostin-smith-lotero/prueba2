#ifndef FERROCARRIL_H
#define FERROCARRIL_H

#include "Propiedad.h"

namespace modelo {

struct Ferrocarril : public Propiedad {
public:
    Ferrocarril(const std::string& nombre, const Escritura& escritura)
        : Propiedad(nombre, escritura, Casilla::Tipo::FERROCARRIL) {}

    int rentaBase(int tirada) const override;
    void alCaer(Juego& juego, Jugador& jugador, int tirada) override;
};

}

#endif //FERROCARRIL_H
