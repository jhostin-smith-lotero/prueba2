#pragma once

#include <string>
#include <unordered_map>

namespace modelo {
class Propiedad;
class Tablero;
}

namespace edd {

class TablaHash {
public:
    void cargarDesdeTablero(const modelo::Tablero& tablero);
    modelo::Propiedad* buscar(const std::string& clave) const;
    bool contiene(const std::string& clave) const;
    void limpiar();

private:
    std::string normalizar(const std::string& clave) const;

    std::unordered_map<std::string, modelo::Propiedad*> indice_;
};

} // namespace edd
