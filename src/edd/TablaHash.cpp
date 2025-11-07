#include "TablaHash.h"

#include "../modelo/Propiedad.h"
#include "../modelo/Tablero.h"
#include "../util/StringUtils.h"

namespace edd {

void TablaHash::cargarDesdeTablero(const modelo::Tablero& tablero) {
    limpiar();
    for (std::size_t i = 0; i < tablero.tamanio(); ++i) {
        auto* propiedad = dynamic_cast<modelo::Propiedad*>(&tablero.casilla(i));
        if (propiedad) {
            indice_[normalizar(propiedad->nombre())] = propiedad;
        }
    }
}

modelo::Propiedad* TablaHash::buscar(const std::string& clave) const {
    auto it = indice_.find(normalizar(clave));
    if (it == indice_.end()) {
        return nullptr;
    }
    return it->second;
}

bool TablaHash::contiene(const std::string& clave) const {
    return indice_.find(normalizar(clave)) != indice_.end();
}

void TablaHash::limpiar() {
    indice_.clear();
}

std::string TablaHash::normalizar(const std::string& clave) const {
    return util::normalizarClave(clave);
}

} // namespace edd
