#include "CalculoRenta.h"

#include "../modelo/Ferrocarril.h"
#include "../modelo/Propiedad.h"
#include "../modelo/Servicio.h"
#include "../modelo/Solar.h"

namespace reglas {

int CalculoRenta::rentaPropiedad(const modelo::Propiedad& propiedad,
                                 int tirada,
                                 int propiedadesGrupo,
                                 int servicios,
                                 int ferrocarriles) {
    if (auto solar = dynamic_cast<const modelo::Solar*>(&propiedad)) {
        return solar->calcularRenta(tirada, propiedadesGrupo, servicios, ferrocarriles);
    }
    if (auto ferrocarril = dynamic_cast<const modelo::Ferrocarril*>(&propiedad)) {
        return ferrocarril->calcularRenta(tirada, propiedadesGrupo, servicios, ferrocarriles);
    }
    if (auto servicio = dynamic_cast<const modelo::Servicio*>(&propiedad)) {
        return servicio->calcularRenta(tirada, propiedadesGrupo, servicios, ferrocarriles);
    }
    return 0;
}

} // namespace reglas
