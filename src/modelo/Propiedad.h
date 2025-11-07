#pragma once

#include "Casilla.h"
#include "Escritura.h"

namespace modelo {

class Jugador;

class Propiedad : public Casilla {
public:
    Propiedad(std::string nombre, Tipo tipo, Escritura escritura);

    Jugador* dueno() const { return dueno_; }
    void asignarDueno(Jugador* jugador);
    void liberar();
    bool estaHipotecada() const { return hipotecada_; }
    void hipotecar() { hipotecada_ = true; }
    void deshipotecar() { hipotecada_ = false; }
    const Escritura& escritura() const { return escritura_; }

    virtual int calcularRenta(int tirada, int cantidadGrupo, int servicios, int ferrocarriles) const = 0;
    void alCaer(juego::Juego& juego, Jugador& jugador, int tirada) override;

protected:
    Jugador* dueno_ = nullptr;
    bool hipotecada_ = false;
    Escritura escritura_;
};

} // namespace modelo
