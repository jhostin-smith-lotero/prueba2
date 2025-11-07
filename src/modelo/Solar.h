#pragma once

#include "Propiedad.h"

#include <string>

namespace modelo {

class Solar : public Propiedad {
public:
    Solar(std::string nombre, std::string color, Escritura escritura);

    const std::string& color() const { return color_; }
    int casas() const { return casas_; }
    bool tieneHotel() const { return hotel_; }

    void construirCasa();
    void venderCasa();
    void construirHotel();
    void venderHotel();

    int calcularRenta(int tirada, int grupoCompleto, int /*servicios*/, int /*ferrocarriles*/) const override;

private:
    std::string color_;
    int casas_ = 0;
    bool hotel_ = false;
};

} // namespace modelo
