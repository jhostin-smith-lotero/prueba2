#ifndef SERVICIO_H
#define SERVICIO_H

#include "Propiedad.h"

namespace modelo {

    /**
     * @class Servicio
     * @brief Propiedad cuyo valor depende del resultado del dado (ej. Empresa de agua/luz).
     */
    struct Servicio : public Propiedad {
    public:
        /**
         * @brief Constructor.
         * @pre escritura válida.
         * @post Servicio inicializado.
         */
        Servicio(const std::string& nombre, const Escritura& escritura)
            : Propiedad(nombre, escritura, Casilla::Tipo::SERVICIO) {}

        /**
         * @brief Calcula la renta multiplicando la tirada.
         * @pre tirada > 0.
         * @post Retorna monto basado en cantidad de servicios y tirada.
         */
        int rentaBase(int tirada) const override;

        /**
         * @brief Lógica al caer sobre el servicio.
         * @pre juego y jugador válidos.
         * @post Compra o pago de renta según corresponda.
         */
        void alCaer(Juego& juego, Jugador& jugador, int tirada) override;
    };

}

#endif //SERVICIO_H
