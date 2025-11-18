#include "Tablero.h"
#include <string>

namespace modelo {

Tablero::Tablero() {
    crearCasillas();
}

Tablero::~Tablero() {
    for (std::size_t i = 0; i < casillas_.size(); ++i) {
        delete casillas_[i];
    }
}

void Tablero::crearCasillas() {
    casillas_.clear();
    casillas_.reserve(40);

    int rentasBasicas[6] = {2, 10, 30, 90, 160, 250};
    int rentasAltas[6] = {6, 30, 90, 270, 400, 550};
    int rentasFerrocarril[6] = {25, 50, 100, 200, 200, 200};
    int rentasServicio[6] = {4, 4, 4, 4, 4, 4};

    Escritura escBarata(60, 30, 50, 50, rentasBasicas);
    Escritura escCara(100, 50, 50, 50, rentasAltas);
    Escritura escFer(200, 100, 0, 0, rentasFerrocarril);
    Escritura escSer(150, 75, 0, 0, rentasServicio);

    for (int i = 0; i < 40; ++i) {
        if (i == 0) {
            casillas_.push_back(new Especial("GO", Especial::GO));
        } else if (i == 10) {
            casillas_.push_back(new Especial("Cárcel", Especial::CARCEL));
        } else if (i == 20) {
            casillas_.push_back(new Especial("Parque", Especial::PARQUE));
        } else if (i == 30) {
            casillas_.push_back(new Especial("Ir a la cárcel", Especial::IR_CARCEL));
        } else if (i % 10 == 5) {
            std::string nombre = "Ferrocarril " + std::to_string(i);
            casillas_.push_back(new Ferrocarril(nombre, escFer));
        } else if (i % 7 == 0) {
            std::string nombre = "Servicio " + std::to_string(i);
            casillas_.push_back(new Servicio(nombre, escSer));
        } else {
            std::string nombre = "Solar " + std::to_string(i);
            std::string color = (i % 2 == 0) ? "Azul" : "Rojo";
            if (i % 3 == 0) {
                casillas_.push_back(new Solar(nombre, escBarata, color));
            } else {
                casillas_.push_back(new Solar(nombre, escCara, color));
            }
        }
    }
}

Casilla* Tablero::obtener(int indice) const {
    if (indice < 0) {
        return nullptr;
    }
    if (static_cast<std::size_t>(indice) >= casillas_.size()) {
        return nullptr;
    }
    return casillas_[static_cast<std::size_t>(indice)];
}

int Tablero::totalCasillas() const {
    return static_cast<int>(casillas_.size());
}

Propiedad* Tablero::obtenerPropiedad(Casilla* casilla) const {
    if (!casilla) {
        return nullptr;
    }
    return dynamic_cast<Propiedad*>(casilla);
}

}
