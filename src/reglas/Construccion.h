#ifndef CONSTRUCCION_H
#define CONSTRUCCION_H

#include <string>

/**
 * @file Construccion.h
 * @brief Funciones para construir casas/hoteles en propiedades del tipo Solar.
 *
 * Dependencias: Solar en modelo.
 */

class Propiedad;
class Jugador;

/**
 * @class Construccion
 * @brief Maneja la lógica de construcción de casas y hoteles.
 */
class Construccion {
public:
    /**
     * @brief Construye una casa en la propiedad indicada (si es posible).
     * @param propiedad Puntero a la propiedad tipo Solar.
     * @param jugador Propietario que desea construir.
     * @pre propiedad y jugador no deben ser null.
     * @pre propiedad debe ser del tipo solar y pertenecer al jugador.
     * @pre propiedad no debe estar hipotecada.
     * @post Si es posible, se incrementa el número de casas y se descuenta el dinero al jugador.
     * @return true si la construcción fue exitosa, false en caso contrario.
     */
    bool construirCasa(Propiedad* propiedad, Jugador* jugador);

    /**
     * @brief Construye un hotel (requerirá 4 casas previas).
     * @param propiedad Puntero a la propiedad tipo Solar.
     * @param jugador Propietario que desea construir.
     * @pre misma que construirCasa además de tener 4 casas.
     * @post Si es posible, se establecen 0 casas y 1 hotel (según implementación) y se descuenta monto.
     * @return true si se construye el hotel; false en caso contrario.
     */
    bool construirHotel(Propiedad* propiedad, Jugador* jugador);
};

#endif // CONSTRUCCION_H
