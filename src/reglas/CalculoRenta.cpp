#include "CalculoRenta.h"
#include <string>
#include <iostream>

// Incluimos headers del modelo (ADAPTAR rutas según tu proyecto)
#include "../modelo/Propiedad.h"
#include "../modelo/Solar.h"
#include "../modelo/Servicio.h"
#include "../modelo/Ferrocarril.h"
#include "../modelo/Jugador.h"

/*
  SUPOSICIONES de interface (adáptalas a la real):
  - Propiedad tiene: virtual std::string getTipo() const; ("solar","servicio","ferrocarril")
                  : virtual int getPrecio() const;
                  : virtual bool isHipotecada() const;
  - Solar tiene: int getNumCasas() const; int getPrecioCasa() const; int getPrecioHotel() const;
  - Servicio: la renta = (dadoSum) * factor (por ejemplo 4 o 10 según poseer ambos servicios)
  - Ferrocarril: renta = tarifa fija * (1 << (numFerrocarrilesPropietario-1))
*/

int CalculoRenta::calcular(const Propiedad* propiedad) const {
    if (!propiedad) return 0;

    std::string tipo = propiedad->getTipo(); // ADAPTAR: método esperado
    if (propiedad->isHipotecada()) {
        return 0;
    }

    if (tipo == "solar") {
        const Solar* s = dynamic_cast<const Solar*>(propiedad);
        if (!s) return 0;
        int base = s->getPrecio(); // precio de la casilla (o renta base)
        int casas = s->getNumCasas();
        // ejemplo simple de tabla de renta:
        if (casas == 0) return base / 10;
        if (casas == 1) return base / 5;
        if (casas == 2) return base / 4;
        if (casas == 3) return base / 3;
        if (casas == 4) return base / 2;
        // hotel
        return base;
    }
    else if (tipo == "servicio") {
        const Servicio* srv = dynamic_cast<const Servicio*>(propiedad);
        if (!srv) return 0;
        // ADAPTAR: suponemos existe método getFactorSegunPropietario(int numServicios)
        int factor = srv->getFactor(); // ej. 4 o 10
        // renta debe multiplicarse por una tirada de dados de quien cae (pero aquí no tenemos el dado)
        // Para el cálculo general se puede devolver el factor y la lógica de cobro la hace Juego/Reglas.
        return factor;
    }
    else if (tipo == "ferrocarril") {
        const Ferrocarril* f = dynamic_cast<const Ferrocarril*>(propiedad);
        if (!f) return 0;
        int base = f->getBaseRent(); // ej. 25, 50, 100, 200 según num ferrocarriles
        return base;
    }

    // Default
    return 0;
}
