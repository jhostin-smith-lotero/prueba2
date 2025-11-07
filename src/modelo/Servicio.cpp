#include "Servicio.h"

#include <utility>

namespace modelo {

Servicio::Servicio(std::string nombre, Escritura escritura, int multiplicadorUno, int multiplicadorDos)
    : Propiedad(std::move(nombre), Tipo::SERVICIO, escritura),
      multiplicadorUno_(multiplicadorUno),
      multiplicadorDos_(multiplicadorDos) {}

int Servicio::calcularRenta(int tirada, int, int servicios, int) const {
    if (servicios >= 2) {
        return tirada * multiplicadorDos_;
    }
    return tirada * multiplicadorUno_;
}

} // namespace modelo
