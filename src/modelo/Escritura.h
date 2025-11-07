#pragma once

#include <array>

namespace modelo {

class Escritura {
public:
    Escritura() = default;

    Escritura(int precio, int hipoteca, int costoCasa, int costoHotel, const std::array<int, 6>& rentas)
        : precio_(precio), hipoteca_(hipoteca), costoCasa_(costoCasa), costoHotel_(costoHotel), rentas_(rentas) {}

    int precio() const { return precio_; }
    int hipoteca() const { return hipoteca_; }
    int costoCasa() const { return costoCasa_; }
    int costoHotel() const { return costoHotel_; }
    int rentaBase() const { return rentas_[0]; }
    int rentaConCasas(int casas) const { return rentas_[casas]; }
    int rentaConHotel() const { return rentas_[5]; }

private:
    int precio_ = 0;
    int hipoteca_ = 0;
    int costoCasa_ = 0;
    int costoHotel_ = 0;
    std::array<int, 6> rentas_{{0, 0, 0, 0, 0, 0}};
};

} // namespace modelo
