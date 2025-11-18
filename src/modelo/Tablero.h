#ifndef TABLERO_H
#define TABLERO_H

#include <vector>
#include <map>
#include <string>
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

    std::vector<Solar*> solaresPorColor(const std::string& color) const;

    struct EstadoPropiedad {
        int indice;
        int dueno; // índice de jugador o -1
        bool hipotecada;
        int casas;
        bool hotel;
    };

    std::vector<EstadoPropiedad> capturarEstado(const std::vector<Jugador>& jugadores) const;
    void restaurarEstado(const std::vector<EstadoPropiedad>& estado, std::vector<Jugador>& jugadores);

    int posicionCarcel() const { return posicionCarcel_; }

    void describirCasilla(int indice) const;

private:
    std::vector<Casilla*> casillas_;
    std::map<std::string, std::vector<Solar*> > grupos_;
    int posicionCarcel_;

    void crearCasillas();
    void registrarColor(Solar* solar);
};

}

#endif //TABLERO_H
