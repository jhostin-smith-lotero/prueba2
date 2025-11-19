#ifndef TABLAHASH_H
#define TABLAHASH_H
#include <map>
#include <string>
#include "../modelo/Tablero.h"

namespace edd {

/**
 * @class TablaHash
 * @brief Índice hash para acceder rápidamente a propiedades según su nombre.
 *
 * Se usa para búsquedas rápidas al escribir nombres de propiedades
 * en comandos como hipotecar, construir, comprar, etc.
 */
struct TablaHash {
public:
    /**
     * @brief Constructor.
     * @pre Ninguna.
     * @post Tabla vacía inicializada.
     */
    TablaHash() {}

    /**
     * @brief Carga todas las propiedades del tablero al índice.
     * @param t Referencia al tablero cargado.
     * @pre El tablero debe estar completamente inicializado.
     * @post El índice contendrá entradas normalizadas de cada propiedad.
     */
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

    /**
     * @brief Busca una propiedad por su nombre.
     * @param clave Nombre escrito por el usuario.
     * @pre Ninguna.
     * @post No modifica el estado de la tabla.
     * @return puntero a Propiedad o nullptr si no existe.
     */
    modelo::Propiedad* buscar(const std::string& clave) const {
        std::string llave = normalizar(clave);
        auto it = indice.find(llave);
        if (it != indice.end()) {
            return it->second;
        }
        return nullptr;
    }

    /**
     * @brief Indica si existe una propiedad con ese nombre.
     * @pre Ninguna.
     * @post No modifica la estructura.
     */
    bool contiene(const std::string& clave) const {
        std::string llave = normalizar(clave);
        return indice.find(llave) != indice.end();
    }

    /**
     * @brief Limpia el contenido de la tabla.
     * @pre Ninguna.
     * @post El índice queda vacío.
     */
    void limpiar() {
        indice.clear();
    }

private:
    std::map<std::string, modelo::Propiedad*> indice;

    /**
     * @brief Convierte un texto a una clave estándar (minúsculas y sin espacios).
     * @pre Ninguna.
     * @post Retorna una versión normalizada del texto.
     */
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
