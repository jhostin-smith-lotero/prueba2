#pragma once

namespace modelo {
class Propiedad;
class Solar;
class Ferrocarril;
class Servicio;
}

namespace reglas {

class CalculoRenta {
public:
    static int rentaPropiedad(const modelo::Propiedad& propiedad,
                              int tirada,
                              int propiedadesGrupo,
                              int servicios,
                              int ferrocarriles);
};

} // namespace reglas
