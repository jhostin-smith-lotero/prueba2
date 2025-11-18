#ifndef TABLAHASH_H
#define TABLAHASH_H
#include <map>
#include <string>
#include "../modelo/Tablero.h"

namespace edd {

struct TablaHash {
public:
    TablaHash() {}

    void cargarDesdeTablero(const modelo::Tablero& t) {
        limpiar();
        int total = t.totalCasillas();
        for (int i = 0; i < total; ++i) {
            modelo::Casilla* casilla = t.obtener(i);
            modelo::Propiedad* propiedad = t.obtenerPropiedad(casilla);
            if (propiedad) {
                std::string clave = normalizar(propiedad->nombre());
                indice[clave] = propiedad;
            }
        }
    }

    modelo::Propiedad* buscar(const std::string& clave) const {
        std::string llave = normalizar(clave);
        auto it = indice.find(llave);
        if (it != indice.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool contiene(const std::string& clave) const {
        std::string llave = normalizar(clave);
        return indice.find(llave) != indice.end();
    }

    void limpiar() {
        indice.clear();
    }

private:
    std::map<std::string, modelo::Propiedad*> indice;

    static std::string normalizar(const std::string& texto) {
        std::string salida;
        for (std::size_t i = 0; i < texto.size(); ++i) {
            char c = texto[i];
            if (c >= 'A' && c <= 'Z') {
                c = static_cast<char>(c - 'A' + 'a');
            }
            if (c != ' ') {
                salida.push_back(c);
            }
        }
        return salida;
    }
};

}

#endif //TABLAHASH_H
