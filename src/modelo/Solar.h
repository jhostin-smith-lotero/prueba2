#ifndef SOLAR_H
#define SOLAR_H

#include "Propiedad.h"

namespace modelo {

    /**
     * @class Solar
     * @brief Propiedad que permite construir casas y hoteles.
     */
    struct Solar : public Propiedad {
    public:
        /**
         * @brief Constructor.
         * @pre escritura válida.
         * @post Solar con 0 casas y sin hotel.
         */
        Solar(const std::string& nombre, const Escritura& escritura, const std::string& color);

        const std::string& color() const { return color_; }
        int casas() const { return casas_; }
        bool hotel() const { return hotel_; }

        /**
         * @brief Construye 1 casa.
         * @pre propiedad no hipotecada, jugador dueño.
         * @post casas_ incrementado en 1.
         */
        void construirCasa();

        /**
         * @brief Construye un hotel.
         * @pre casas_ == 4.
         * @post casas_ = 0, hotel_ = true.
         */
        void construirHotel();

        /**
         * @brief Establece edificación manualmente.
         * @pre casas entre 0 y 4.
         * @post casas_ y hotel_ actualizados.
         */
        void fijarEdificaciones(int casas, bool hotel);

        /**
         * @brief Calcula renta del solar.
         * @pre tirada >= 0.
         * @post Retorna renta según casas u hotel.
         */
        int rentaBase(int tirada) const override;

        /**
         * @brief Lógica al caer en el solar.
         * @pre juego y jugador válidos.
         * @post Compra o pago de renta.
         */
        void alCaer(Juego& juego, Jugador& jugador, int tirada) override;

    private:
        std::string color_;
        int casas_;
        bool hotel_;
    };

}

#endif //SOLAR_H
