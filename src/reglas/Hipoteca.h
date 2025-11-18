#ifndef HIPOTECA_H
#define HIPOTECA_H

#include <string>
#include "../modelo/Propiedad.h"
#include "../modelo/Jugador.h"

/**
 * @class Hipoteca
 * @brief Gestiona hipotecar y cancelar hipoteca en propiedades.
 */
class Hipoteca {
public:
    /**
     * @brief Hipoteca una propiedad, entregando al dueño un porcentaje del precio.
     * @param propiedad Puntero a la propiedad a hipotecar.
     * @param jugador Jugador que solicita la hipoteca.
     * @pre propiedad y jugador no nulos.
     * @pre propiedad debe pertenecer al jugador y no estar ya hipotecada.
     * @post Si se hipoteca, la propiedad queda marcada y el jugador recibe dinero.
     * @return true si se realizó la hipoteca; false en caso contrario.
     */
    bool hipotecar(modelo::Propiedad* propiedad, modelo::Jugador* jugador);

    /**
     * @brief Cancela una hipoteca (pagar valor más interés).
     * @param propiedad Puntero a la propiedad cuya hipoteca se cancela.
     * @param jugador Jugador que cancela la hipoteca.
     * @pre propiedad debe estar hipotecada y pertenecer al jugador.
     * @post Si se cancela, propiedad ya no está hipotecada y se descuenta dinero del jugador.
     * @return true si se realizó la cancelación; false en caso contrario.
     */
    bool cancelarHipoteca(modelo::Propiedad* propiedad, modelo::Jugador* jugador);
};

#endif // HIPOTECA_H
