#pragma once

#include "Propiedad.h"

namespace modelo {

class Ferrocarril : public Propiedad {
public:
    explicit Ferrocarril(std::string nombre, Escritura escritura);

    int calcularRenta(int /*tirada*/, int /*grupo*/, int /*servicios*/, int ferrocarriles) const override;
};

} // namespace modelo
