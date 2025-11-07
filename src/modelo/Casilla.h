#pragma once

#include <string>

namespace juego {
class Juego;
}

namespace modelo {

class Jugador;

class Casilla {
public:
    enum class Tipo { ESPECIAL, SOLAR, FERROCARRIL, SERVICIO };

    explicit Casilla(std::string nombre, Tipo tipo) : nombre_(std::move(nombre)), tipo_(tipo) {}
    virtual ~Casilla() = default;

    const std::string& nombre() const { return nombre_; }
    Tipo tipo() const { return tipo_; }

    virtual void alCaer(juego::Juego& juego, Jugador& jugador, int tirada) = 0;

protected:
    std::string nombre_;
    Tipo tipo_;
};

} // namespace modelo
