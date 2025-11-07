#include "Carta.h"

#include <utility>

namespace modelo {

Carta::Carta(std::string texto, Accion accion, int valor, int destino, bool conservar)
    : texto_(std::move(texto)), accion_(accion), valor_(valor), destino_(destino), conservar_(conservar) {}

} // namespace modelo
