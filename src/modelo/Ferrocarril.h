#ifndef FERROCARRIL_H
#define FERROCARRIL_H

#include "Propiedad.h"

namespace modelo {

    /**
     * @class Ferrocarril
     * @brief Propiedad especial cuyo valor de renta depende del número de ferrocarriles que posea el propietario.
     */
    struct Ferrocarril : public Propiedad {
    public:
        /**
         * @brief Constructor.
         * @pre escritura válida.
         * @post Propiedad tipo FERROCARRIL creada.
         */
        Ferrocarril(const std::string& nombre, const Escritura& escritura)
            : Propiedad(nombre, escritura, Casilla::Tipo::FERROCARRIL) {}

        /**
         * @brief Calcula la renta base del ferrocarril.
         * @pre tirada >= 0.
         * @post Retorna el valor de renta según la cantidad de ferrocarriles del dueño.
         */
        int rentaBase(int tirada) const override;

        /**
         * @brief Lógica al caer en la propiedad.
         * @pre juego y jugador válidos.
         * @post Se ejecuta compra o cobro de renta según estado de propiedad.
         */
        void alCaer(Juego& juego, Jugador& jugador, int tirada) override;
    };

}

#endif //FERROCARRIL_H
