#ifndef HIPOTECA_H
#define HIPOTECA_H

#include <string>

/**
 * @file Hipoteca.h
 * @brief Lógica de hipoteca para propiedades (poner/quitar hipoteca).
 */

class Propiedad;
class Jugador;

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
    bool hipotecar(Propiedad* propiedad, Jugador* jugador);

    /**
     * @brief Cancela una hipoteca (pagar valor más interés).
     * @param propiedad Puntero a la propiedad cuya hipoteca se cancela.
     * @param jugador Jugador que cancela la hipoteca.
     * @pre propiedad debe estar hipotecada y pertenecer al jugador.
     * @post Si se cancela, propiedad ya no está hipotecada y se descuenta dinero del jugador.
     * @return true si se realizó la cancelación; false en caso contrario.
     */
    bool cancelarHipoteca(Propiedad* propiedad, Jugador* jugador);
};

#endif // HIPOTECA_H
