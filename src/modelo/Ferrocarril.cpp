#include "Ferrocarril.h"

#include <utility>

namespace modelo {

Ferrocarril::Ferrocarril(std::string nombre, Escritura escritura)
    : Propiedad(std::move(nombre), Tipo::FERROCARRIL, escritura) {}

int Ferrocarril::calcularRenta(int, int, int, int ferrocarriles) const {
    if (ferrocarriles <= 0) {
        ferrocarriles = 1;
    }
    int renta = escritura_.rentaBase();
    for (int i = 1; i < ferrocarriles; ++i) {
        renta *= 2;
    }
    return renta;
}

} // namespace modelo
