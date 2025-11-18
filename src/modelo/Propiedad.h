#ifndef PROPIEDAD_H
#define PROPIEDAD_H

#include "Casilla.h"
#include "Escritura.h"

namespace modelo {

struct Propiedad : public Casilla {
public:
    Propiedad(const std::string& nombre, const Escritura& escritura, Casilla::Tipo tipoPropiedad)
        : Casilla(nombre, tipoPropiedad), escritura_(escritura), dueno_(nullptr), hipotecada_(false) {}

    Jugador* dueno() const { return dueno_; }
    bool estaHipotecada() const { return hipotecada_; }
    const Escritura& datos() const { return escritura_; }

    bool disponible() const { return dueno_ == nullptr; }

    void asignarDueno(Jugador* jugador) { dueno_ = jugador; }

    void hipotecar() { hipotecada_ = true; }
    void deshipotecar() { hipotecada_ = false; }

    virtual int rentaBase(int tirada) const { (void)tirada; return escritura_.rentaBase(); }

    void alCaer(Juego& juego, Jugador& jugador, int tirada) override;

protected:
    Escritura escritura_;
    Jugador* dueno_;
    bool hipotecada_;
};

}

#endif //PROPIEDAD_H
