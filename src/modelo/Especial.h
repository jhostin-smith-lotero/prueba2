#ifndef ESPECIAL_H
#define ESPECIAL_H

#include "Casilla.h"
#include <string>

namespace modelo {

    /**
     * @class Especial
     * @brief Casillas especiales del tablero (GO, cárcel, impuestos, arca, casualidad, parque gratuito).
     *
     * Cada casilla especial tiene reglas únicas que se aplican cuando
     * un jugador cae en ella.
     */
    struct Especial : public Casilla {
    public:
        /**
         * @brief Enumeración de tipos específicos.
         */
        enum TipoEspecial { GO, CARCEL, IR_CARCEL, IMPUESTO, CASUALIDAD, ARCA, PARQUE };

        /**
         * @brief Constructor.
         * @param nombre Nombre visible de la casilla.
         * @param tipo Tipo especial.
         * @param monto Monto de impuesto (si aplica).
         * @pre nombre no vacío.
         * @post Casilla especial inicializada correctamente.
         */
        Especial(const std::string& nombre, TipoEspecial tipo, int monto = 0);

        TipoEspecial tipoEspecial() const { return tipoEspecial_; }
        int montoImpuesto() const { return monto_; }

        /**
         * @brief Lógica al caer en esta casilla.
         * @pre juego y jugador deben ser válidos.
         * @post Se aplican acciones según el tipo (pagar impuesto, ir a la cárcel, robar carta, etc.).
         */
        void alCaer(Juego& juego, Jugador& jugador, int tirada) override;

    private:
        TipoEspecial tipoEspecial_;
        int monto_;
    };

}

#endif //ESPECIAL_H
