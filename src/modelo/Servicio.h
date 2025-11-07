#pragma once

#include "Propiedad.h"

namespace modelo {

class Servicio : public Propiedad {
public:
    Servicio(std::string nombre, Escritura escritura, int multiplicadorUno, int multiplicadorDos);

    int multiplicadorUno() const { return multiplicadorUno_; }
    int multiplicadorDos() const { return multiplicadorDos_; }

    int calcularRenta(int tirada, int /*grupo*/, int servicios, int /*ferrocarriles*/) const override;

private:
    int multiplicadorUno_ = 4;
    int multiplicadorDos_ = 10;
};

} // namespace modelo
