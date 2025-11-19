#ifndef HIPOTECA_H
#define HIPOTECA_H

#include <string>
#include "../modelo/Propiedad.h"
#include "../modelo/Jugador.h"

/**
 * @class Hipoteca
 * @brief Gestiona el sistema de hipoteca del juego Monopoly.
 *
 * Reglas aplicadas:
 * - Hipotecar otorga al jugador el valor de hipoteca de la escritura.
 * - Cancelar hipoteca cuesta hipoteca * 1.10 (interés del 10%).
 * - No se puede hipotecar una propiedad con casas u hoteles (regla controlada en Solar).
 */
class Hipoteca {
public:

    /**
     * @brief Hipoteca una propiedad otorgando dinero al jugador.
     *
     * @param propiedad Propiedad a hipotecar.
     * @param jugador Jugador dueño de la propiedad.
     *
     * @pre propiedad != nullptr.
     * @pre jugador != nullptr.
     * @pre propiedad->dueno() == jugador.
     * @pre propiedad no debe estar hipotecada.
     *
     * @post propiedad queda hipotecada.
     * @post jugador recibe propiedad->datos().hipoteca().
     *
     * @return true si se realizó la hipoteca, false en caso contrario.
     */
    bool hipotecar(modelo::Propiedad* propiedad, modelo::Jugador* jugador);

    /**
     * @brief Cancela la hipoteca de la propiedad.
     *
     * @param propiedad Propiedad cuya hipoteca se cancelará.
     * @param jugador Jugador dueño de la propiedad.
     *
     * @pre propiedad != nullptr.
     * @pre jugador != nullptr.
     * @pre propiedad->estaHipotecada() == true.
     * @pre propiedad->dueno() == jugador.
     * @pre jugador tiene suficiente dinero para cancelar.
     *
     * @post propiedad->estaHipotecada() == false.
     * @post jugador paga hipoteca * 1.10.
     *
     * @return true si el jugador logró pagar y se canceló la hipoteca.
     */
    bool cancelarHipoteca(modelo::Propiedad* propiedad, modelo::Jugador* jugador);
};

#endif // HIPOTECA_H
