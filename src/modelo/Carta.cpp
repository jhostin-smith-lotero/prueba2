#include "Carta.h"
namespace modelo {

    Carta::Carta(const std::string& texto, Accion accion, int valor, int destino, bool conservar)
        : texto_(texto), accion_(accion), valor_(valor), destino_(destino), conservar_(conservar) {}

}