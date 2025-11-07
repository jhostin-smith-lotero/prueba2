#include "Tablero.h"

#include "Casilla.h"

#include <stdexcept>

namespace modelo {

void Tablero::limpiar() {
    casillas_.clear();
}

void Tablero::agregarCasilla(std::unique_ptr<Casilla> casilla) {
    casillas_.push_back(std::move(casilla));
}

std::size_t Tablero::tamanio() const {
    return casillas_.size();
}

Casilla& Tablero::casilla(std::size_t indice) const {
    if (indice >= casillas_.size()) {
        throw std::out_of_range("Índice de casilla inválido");
    }
    return *casillas_[indice];
}

int Tablero::avanzar(int posicionActual, int pasos) const {
    if (casillas_.empty()) {
        return 0;
    }
    int nuevo = (posicionActual + pasos) % static_cast<int>(casillas_.size());
    if (nuevo < 0) {
        nuevo += static_cast<int>(casillas_.size());
    }
    return nuevo;
}

bool Tablero::pasoPorSalida(int posicionActual, int pasos) const {
    if (casillas_.empty()) {
        return false;
    }
    int total = posicionActual + pasos;
    return total >= static_cast<int>(casillas_.size());
}

} // namespace modelo
