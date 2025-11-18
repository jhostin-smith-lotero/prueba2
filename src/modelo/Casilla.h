#ifndef CASILLA_H
#define CASILLA_H
#include <string>
#include "Jugador.h"

class Juego;

namespace modelo {

struct Casilla {
public:
    enum class Tipo { ESPECIAL, SOLAR, FERROCARRIL, SERVICIO };

    explicit Casilla(const std::string& nombre, Tipo tipo) : nombre_(nombre), tipo_(tipo) {}
    virtual ~Casilla() {}

    const std::string& nombre() const { return nombre_; }
    Tipo tipo() const { return tipo_; }

    virtual void alCaer(Juego& juego, Jugador& jugador, int tirada) = 0;

protected:
    std::string nombre_;
    Tipo tipo_;
};

}
#endif //CASILLA_H
