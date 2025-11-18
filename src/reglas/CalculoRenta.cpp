#include "CalculoRenta.h"
#include "../modelo/Propiedad.h"

int CalculoRenta::calcular(const modelo::Propiedad* propiedad, int tirada) const {
    if (!propiedad) {
        return 0;
    }
    return propiedad->rentaBase(tirada);
}
