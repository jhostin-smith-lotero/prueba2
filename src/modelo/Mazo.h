#pragma once

#include "../edd/Cola.h"
#include "Carta.h"

#include <vector>

namespace modelo {

class Mazo {
public:
    void cargar(const std::vector<Carta>& cartas);
    Carta robar();
    void ponerAlFondo(const Carta& carta);
    bool vacio() const;
    std::size_t cantidad() const;
    const edd::Cola<Carta>& datos() const { return cartas_; }
    void establecerDatos(const edd::Cola<Carta>& datos);

private:
    edd::Cola<Carta> cartas_;
};

} // namespace modelo
