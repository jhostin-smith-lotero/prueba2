#ifndef CALCULORENTA_H
#define CALCULORENTA_H

/**
 * @file CalculoRenta.h
 * @brief Cálculo de renta para diferentes tipos de propiedad (solar, servicio, ferrocarril).
 *
 * Dependencias: se asume que en /src/modelo existen clases:
 * - Propiedad (interfaz base)
 * - Solar : Propiedad
 * - Servicio : Propiedad
 * - Ferrocarril : Propiedad
 *
 * ADAPTAR: si los nombres de métodos difieren, actualizar las llamadas en .cpp
 */

namespace modelo { struct Propiedad; }

/**
 * @class CalculoRenta
 * @brief Calcula la renta a cobrar cuando un jugador cae en una propiedad.
 */
class CalculoRenta {
public:
    /**
     * @brief Calcula la renta en función del tipo de propiedad y su estado.
     * @param propiedad Puntero a la propiedad evaluada.
     * @pre propiedad no debe ser nullptr.
     * @pre La propiedad debe tener propietario distinto del jugador que cae (ver lógica superior).
     * @post Retorna la cantidad de dinero que debe pagar el jugador visitante.
     * @return entero con el monto de renta.
     */
    int calcular(const modelo::Propiedad* propiedad, int tirada) const;
};

#endif // CALCULORENTA_H
