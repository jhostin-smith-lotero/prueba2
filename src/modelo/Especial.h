#ifndef ESPECIAL_H
#define ESPECIAL_H

#include "Casilla.h"
#include <string>

namespace modelo {

struct Especial : public Casilla {
public:
    enum TipoEspecial { GO, CARCEL, IR_CARCEL, IMPUESTO, CASUALIDAD, ARCA, PARQUE };

    Especial(const std::string& nombre, TipoEspecial tipo, int monto = 0);

    TipoEspecial tipoEspecial() const { return tipoEspecial_; }
    int montoImpuesto() const { return monto_; }

    void alCaer(Juego& juego, Jugador& jugador, int tirada) override;

private:
    TipoEspecial tipoEspecial_;
    int monto_;
};

}

#endif //ESPECIAL_H
