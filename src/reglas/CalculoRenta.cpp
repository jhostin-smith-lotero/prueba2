#include "CalculoRenta.h"
#include "../modelo/Propiedad.h"

int CalculoRenta::calcular(const modelo::Propiedad* propiedad) const {
    if (!propiedad) {
        return 0;
    }
    return propiedad->rentaBase(0);
}
