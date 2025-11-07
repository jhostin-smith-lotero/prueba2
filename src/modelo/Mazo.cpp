#include "Mazo.h"

#include <stdexcept>

namespace modelo {

void Mazo::cargar(const std::vector<Carta>& cartas) {
    cartas_.clear();
    for (const auto& carta : cartas) {
        cartas_.enqueue(carta);
    }
}

Carta Mazo::robar() {
    Carta carta;
    if (!cartas_.dequeue(carta)) {
        throw std::runtime_error("El mazo está vacío");
    }
    return carta;
}

void Mazo::ponerAlFondo(const Carta& carta) {
    cartas_.enqueue(carta);
}

bool Mazo::vacio() const {
    return cartas_.vacia();
}

std::size_t Mazo::cantidad() const {
    return cartas_.tam();
}

void Mazo::establecerDatos(const edd::Cola<Carta>& datos) {
    cartas_ = datos;
}

} // namespace modelo
