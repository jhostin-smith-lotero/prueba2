#ifndef TABLERO_H
#define TABLERO_H

#include <vector>
#include <map>
#include <string>
#include "Casilla.h"
#include "Solar.h"
#include "Ferrocarril.h"
#include "Servicio.h"
#include "Especial.h"

namespace modelo {

    /**
     * @class Tablero
     * @brief Contiene todas las casillas de Monopoly y su estructura lógica.
     */
    struct Tablero {
    public:
        /**
         * @brief Constructor.
         * @pre Ninguna.
         * @post Crea todas las casillas del tablero estándar.
         */
        Tablero();

        /**
         * @brief Destructor.
         * @post Libera toda la memoria de casillas creadas dinámicamente.
         */
        ~Tablero();

        /**
         * @brief Obtiene una casilla por índice.
         * @pre 0 <= indice < totalCasillas().
         * @post No modifica el estado.
         */
        Casilla* obtener(int indice) const;

        /**
         * @return Número total de casillas.
         */
        int totalCasillas() const;

        /**
         * @brief Convierte una casilla a propiedad si aplica.
         * @return Puntero a propiedad o nullptr.
         */
        Propiedad* obtenerPropiedad(Casilla* casilla) const;

        /**
         * @brief Retorna los solares de un mismo color.
         * @pre color válido existente.
         * @post No modifica estado.
         */
        std::vector<Solar*> solaresPorColor(const std::string& color) const;

        /**
         * @struct EstadoPropiedad
         * @brief Representa un snapshot del estado de propiedades.
         */
        struct EstadoPropiedad {
            int indice;
            int dueno;
            bool hipotecada;
            int casas;
            bool hotel;
        };

        /**
         * @brief Captura el estado actual de todas las propiedades.
         * @post Devuelve vector representando estado serializable.
         */
        std::vector<EstadoPropiedad> capturarEstado(const std::vector<Jugador>& jugadores) const;

        /**
         * @brief Restaura estado anterior.
         * @post Propiedades recuperan dueño, hipoteca, edificaciones.
         */
        void restaurarEstado(const std::vector<EstadoPropiedad>& estado, std::vector<Jugador>& jugadores);

        /**
         * @return Índice de la cárcel.
         */
        int posicionCarcel() const { return posicionCarcel_; }

        /**
         * @brief Describe una casilla (para debug).
         */
        void describirCasilla(int indice) const;

    private:
        std::vector<Casilla*> casillas_;
        std::map<std::string, std::vector<Solar*> > grupos_;
        int posicionCarcel_;

        /**
         * @brief Crea todas las casillas del tablero estándar.
         * @post casillas_ inicializado completamente.
         */
        void crearCasillas();

        /**
         * @brief Agrupa solares por color.
         * @pre solar válido.
         * @post Grupo actualizado.
         */
        void registrarColor(Solar* solar);
    };

}

#endif //TABLERO_H
