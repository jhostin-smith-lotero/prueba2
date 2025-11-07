#pragma once

#include "Casilla.h"

namespace modelo {

class Especial : public Casilla {
public:
    enum class Categoria {
        SALIDA,
        ARCA,
        CASUALIDAD,
        IMPUESTO,
        IMPUESTO_LUJO,
        CARCEL,
        IR_CARCEL,
        ESTACIONAMIENTO,
        NADA
    };

    Especial(std::string nombre, Categoria categoria, int monto = 0);

    Categoria categoria() const { return categoria_; }
    int monto() const { return monto_; }

    void alCaer(juego::Juego& juego, Jugador& jugador, int tirada) override;

private:
    Categoria categoria_;
    int monto_ = 0;
};

} // namespace modelo
