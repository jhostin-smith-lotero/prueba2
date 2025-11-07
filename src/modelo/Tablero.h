#pragma once

#include "Casilla.h"

#include <memory>
#include <vector>

namespace modelo {

class Tablero {
public:
    void limpiar();
    void agregarCasilla(std::unique_ptr<Casilla> casilla);
    std::size_t tamanio() const;
    Casilla& casilla(std::size_t indice) const;
    int avanzar(int posicionActual, int pasos) const;
    bool pasoPorSalida(int posicionActual, int pasos) const;

private:
    std::vector<std::unique_ptr<Casilla>> casillas_;
};

} // namespace modelo
