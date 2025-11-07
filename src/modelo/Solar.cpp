#include "Solar.h"

#include <utility>

namespace modelo {

Solar::Solar(std::string nombre, std::string color, Escritura escritura)
    : Propiedad(std::move(nombre), Tipo::SOLAR, escritura), color_(std::move(color)) {}

void Solar::construirCasa() {
    if (hotel_) {
        return;
    }
    ++casas_;
}

void Solar::venderCasa() {
    if (casas_ > 0) {
        --casas_;
    }
}

void Solar::construirHotel() {
    casas_ = 0;
    hotel_ = true;
}

void Solar::venderHotel() {
    hotel_ = false;
}

int Solar::calcularRenta(int, int grupoCompleto, int, int) const {
    if (hotel_) {
        return escritura_.rentaConHotel();
    }
    if (casas_ > 0) {
        return escritura_.rentaConCasas(casas_);
    }
    if (grupoCompleto > 0) {
        return escritura_.rentaBase() * 2;
    }
    return escritura_.rentaBase();
}

} // namespace modelo
