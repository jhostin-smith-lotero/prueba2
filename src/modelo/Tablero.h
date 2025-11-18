#ifndef TABLERO_H
#define TABLERO_H

#include <vector>
#include "Casilla.h"
#include "Solar.h"
#include "Ferrocarril.h"
#include "Servicio.h"
#include "Especial.h"

namespace modelo {

struct Tablero {
public:
    Tablero();
    ~Tablero();

    Casilla* obtener(int indice) const;
    int totalCasillas() const;

    Propiedad* obtenerPropiedad(Casilla* casilla) const;

private:
    std::vector<Casilla*> casillas_;

    void crearCasillas();
};

}

#endif //TABLERO_H
