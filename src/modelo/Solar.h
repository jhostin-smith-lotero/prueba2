#ifndef SOLAR_H
#define SOLAR_H

#include "Propiedad.h"

namespace modelo {

struct Solar : public Propiedad {
public:
    Solar(const std::string& nombre, const Escritura& escritura, const std::string& color);

    const std::string& color() const { return color_; }
    int casas() const { return casas_; }
    bool hotel() const { return hotel_; }

    void construirCasa();
    void construirHotel();
    void fijarEdificaciones(int casas, bool hotel);

    int rentaBase(int tirada) const override;
    void alCaer(Juego& juego, Jugador& jugador, int tirada) override;

private:
    std::string color_;
    int casas_;
    bool hotel_;
};

}

#endif //SOLAR_H
