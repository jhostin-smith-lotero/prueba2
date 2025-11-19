#ifndef CONSTRUCCION_H
#define CONSTRUCCION_H

#include <string>
#include "../modelo/Propiedad.h"
#include "../modelo/Solar.h"
#include "../modelo/Jugador.h"

/**
 * @file Construccion.h
 * @brief Permite construir casas y hoteles en propiedades tipo Solar.
 *
 * Reglas aplicadas:
 * - Solo solares pueden tener edificaciones.
 * - Debe ser dueño del solar.
 * - No se puede construir si la propiedad está hipotecada.
 * - Para hotel se requieren 4 casas previas.
 */
class Construccion {
public:

    /**
     * @brief Construye una casa en un solar.
     *
     * @param propiedad Debe ser un Solar.
     * @param jugador Dueño del solar.
     *
     * @pre propiedad != nullptr.
     * @pre jugador != nullptr.
     * @pre propiedad es de tipo Solar (dynamic_cast válido).
     * @pre propiedad->dueno() == jugador.
     * @pre propiedad no está hipotecada.
     * @pre solar->casas() < 4.
     * @pre jugador tiene fondos suficientes para pagar costoCasa.
     *
     * @post solar->casas() incrementa en 1.
     * @post jugador->dinero() disminuye en escritura.costoCasa().
     *
     * @return true si la casa fue construida.
     */
    bool construirCasa(modelo::Propiedad* propiedad, modelo::Jugador* jugador);

    /**
     * @brief Construye un hotel en un solar.
     *
     * @param propiedad Debe ser un Solar.
     * @param jugador Dueño del solar.
     *
     * @pre propiedad != nullptr.
     * @pre jugador != nullptr.
     * @pre propiedad es Solar (dynamic_cast).
     * @pre propiedad->dueno() == jugador.
     * @pre solar->casas() == 4.
     * @pre solar->hotel() == false.
     * @pre propiedad no está hipotecada.
     * @pre jugador tiene suficiente dinero para pagar costoHotel.
     *
     * @post solar->casas() = 0.
     * @post solar->hotel() = true.
     * @post jugador->dinero() disminuye en costoHotel.
     *
     * @return true si el hotel fue construido.
     */
    bool construirHotel(modelo::Propiedad* propiedad, modelo::Jugador* jugador);
};

#endif // CONSTRUCCION_H
