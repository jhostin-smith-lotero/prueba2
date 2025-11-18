#include "Construccion.h"
#include <iostream>
#include "../modelo/Propiedad.h"
#include "../modelo/Solar.h"
#include "../modelo/Jugador.h"

/*
 SUPOSICIONES método Solar:
  - int getNumCasas() const;
  - void setNumCasas(int);
  - int getPrecioCasa() const;
  - int getPrecioHotel() const;
  - bool belongsTo(const Jugador* jugador) const; // o check propietario por nombre
  - bool isHipotecada() const;
*/

bool Construccion::construirCasa(Propiedad* propiedad, Jugador* jugador) {
    if (!propiedad || !jugador) return false;

    if (propiedad->getTipo() != "solar") return false;
    Solar* s = dynamic_cast<Solar*>(propiedad);
    if (!s) return false;

    // verificar propiedad
    if (!s->belongsTo(jugador)) return false; // ADAPTAR: cambia según API
    if (s->isHipotecada()) return false;

    int precioCasa = s->getPrecioCasa();
    if (jugador->getDinero() < precioCasa) return false;

    // descuento y asignación
    jugador->pagar(precioCasa); // ADAPTAR: usa el método real de jugador o Banco
    s->setNumCasas(s->getNumCasas() + 1);
    std::cout << jugador->getNombre() << " construyó una casa en " << s->getNombre() << ".\n";
    return true;
}

bool Construccion::construirHotel(Propiedad* propiedad, Jugador* jugador) {
    if (!propiedad || !jugador) return false;
    if (propiedad->getTipo() != "solar") return false;
    Solar* s = dynamic_cast<Solar*>(propiedad);
    if (!s) return false;
    if (!s->belongsTo(jugador)) return false;
    if (s->isHipotecada()) return false;

    if (s->getNumCasas() < 4) return false;

    int precioHotel = s->getPrecioHotel();
    if (jugador->getDinero() < precioHotel) return false;

    jugador->pagar(precioHotel);
    s->setNumCasas(0);
    s->setTieneHotel(true); // ADAPTAR: si tu Solar maneja hotel de otro modo
    std::cout << jugador->getNombre() << " construyó un hotel en " << s->getNombre() << ".\n";
    return true;
}
