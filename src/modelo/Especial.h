#ifndef ESPECIAL_H
#define ESPECIAL_H

#include "Casilla.h"
#include <string>

namespace modelo {

struct Especial : public Casilla {
public:
    enum TipoEspecial { GO, CARCEL, IR_CARCEL, IMPUESTO, CASUALIDAD, ARCA, PARQUE };

    Especial(const std::string& nombre, TipoEspecial tipo);

    TipoEspecial tipoEspecial() const { return tipoEspecial_; }

    void alCaer(Juego& juego, Jugador& jugador, int tirada) override;

private:
    TipoEspecial tipoEspecial_;
};

}

#endif //ESPECIAL_H
