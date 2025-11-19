#ifndef CASILLA_H
#define CASILLA_H
#include <string>
#include "Jugador.h"

class Juego;

namespace modelo {

    /**
     * @class Casilla
     * @brief Clase base para todas las casillas del tablero.
     */
    struct Casilla {
    public:
        /**
         * @brief Tipos de casillas posibles.
         */
        enum class Tipo { ESPECIAL, SOLAR, FERROCARRIL, SERVICIO };

        /**
         * @brief Constructor.
         * @pre nombre no vacío.
         * @post Casilla creada con tipo definido.
         */
        explicit Casilla(const std::string& nombre, Tipo tipo) : nombre_(nombre), tipo_(tipo) {}

        virtual ~Casilla() {}

        const std::string& nombre() const { return nombre_; }
        Tipo tipo() const { return tipo_; }

        /**
         * @brief Acción al caer en esta casilla.
         * @pre juego y jugador válidos.
         * @post Se ejecuta el comportamiento particular de cada tipo.
         */
        virtual void alCaer(Juego& juego, Jugador& jugador, int tirada) = 0;

    protected:
        std::string nombre_;
        Tipo tipo_;
    };

}

#endif //CASILLA_H
