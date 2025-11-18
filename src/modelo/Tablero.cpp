#include "Tablero.h"
#include <string>
#include <cstdio>
#include <cctype>
#include <iostream>

namespace modelo {

static int extraerEnteroLista(const std::string& linea, std::size_t& pos) {
    int valor = 0;
    while (pos < linea.size() && !(linea[pos] >= '0' && linea[pos] <= '9')) {
        pos++;
    }
    while (pos < linea.size() && (linea[pos] >= '0' && linea[pos] <= '9')) {
        valor = valor * 10 + (linea[pos] - '0');
        pos++;
    }
    return valor;
}

static void parsearRentas(const std::string& linea, int rentas[6]) {
    for (int i = 0; i < 6; ++i) rentas[i] = 0;
    std::size_t pos = 0;
    int idx = 0;
    while (pos < linea.size() && idx < 6) {
        int valor = extraerEnteroLista(linea, pos);
        if (valor > 0) {
            rentas[idx++] = valor;
        } else {
            pos++;
        }
    }
    if (idx > 0) {
        int ultimo = rentas[idx - 1];
        for (int i = idx; i < 6; ++i) {
            rentas[i] = ultimo;
        }
    }
}

Tablero::Tablero() : posicionCarcel_(10) {
    crearCasillas();
}

Tablero::~Tablero() {
    for (std::size_t i = 0; i < casillas_.size(); ++i) {
        delete casillas_[i];
    }
}

void Tablero::registrarColor(Solar* solar) {
    if (!solar) return;
    grupos_[solar->color()].push_back(solar);
}

void Tablero::crearCasillas() {
    casillas_.clear();
    grupos_.clear();

    FILE* f = std::fopen("data/tablero.txt", "r");
    if (!f) {
        return;
    }
    char buffer[512];
    std::string tipo;
    std::string nombre;
    std::string grupo;
    int precio = 0;
    int monto = 0;
    int rentas[6];
    for (int i = 0; i < 6; ++i) rentas[i] = 0;

    while (std::fgets(buffer, sizeof(buffer), f)) {
        std::string linea(buffer);
        if (linea.find("\"tipo\"") != std::string::npos) {
            std::size_t inicio = linea.find("\"");
            inicio = linea.find("\"", inicio + 1);
            std::size_t fin = linea.find("\"", inicio + 1);
            tipo = linea.substr(inicio + 1, fin - inicio - 1);
        }
        if (linea.find("\"nombre\"") != std::string::npos) {
            std::size_t inicio = linea.find(":");
            std::size_t comillas1 = linea.find("\"", inicio);
            std::size_t comillas2 = linea.find("\"", comillas1 + 1);
            nombre = linea.substr(comillas1 + 1, comillas2 - comillas1 - 1);
        }
        if (linea.find("\"grupo\"") != std::string::npos) {
            std::size_t inicio = linea.find(":");
            std::size_t comillas1 = linea.find("\"", inicio);
            std::size_t comillas2 = linea.find("\"", comillas1 + 1);
            grupo = linea.substr(comillas1 + 1, comillas2 - comillas1 - 1);
        }
        if (linea.find("\"precio\"") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            precio = extraerEnteroLista(linea, pos);
        }
        if (linea.find("\"monto\"") != std::string::npos) {
            std::size_t pos = linea.find(":");
            pos++;
            monto = extraerEnteroLista(linea, pos);
        }
        if (linea.find("renta") != std::string::npos || linea.find("rentas") != std::string::npos) {
            parsearRentas(linea, rentas);
        }

        if (linea.find("}") != std::string::npos) {
            if (tipo == "SOLAR") {
                Escritura esc(precio, precio / 2, 50, 50, rentas);
                Solar* s = new Solar(nombre, esc, grupo);
                casillas_.push_back(s);
                registrarColor(s);
            } else if (tipo == "FERROCARRIL") {
                Escritura esc(precio, precio / 2, 0, 0, rentas);
                casillas_.push_back(new Ferrocarril(nombre, esc));
            } else if (tipo == "SERVICIO") {
                Escritura esc(precio, precio / 2, 0, 0, rentas);
                casillas_.push_back(new Servicio(nombre, esc));
            } else if (tipo == "GO") {
                casillas_.push_back(new Especial(nombre, Especial::GO));
            } else if (tipo == "CARCEL") {
                posicionCarcel_ = static_cast<int>(casillas_.size());
                casillas_.push_back(new Especial(nombre, Especial::CARCEL));
            } else if (tipo == "IR_CARCEL") {
                casillas_.push_back(new Especial(nombre, Especial::IR_CARCEL));
            } else if (tipo == "IMPUESTO") {
                casillas_.push_back(new Especial(nombre, Especial::IMPUESTO, monto));
            } else if (tipo == "CASUALIDAD") {
                casillas_.push_back(new Especial(nombre, Especial::CASUALIDAD));
            } else if (tipo == "ARCA") {
                casillas_.push_back(new Especial(nombre, Especial::ARCA));
            } else if (tipo == "PARQUE") {
                casillas_.push_back(new Especial(nombre, Especial::PARQUE));
            }
            tipo.clear();
            nombre.clear();
            grupo.clear();
            precio = 0;
            monto = 0;
            for (int i = 0; i < 6; ++i) rentas[i] = 0;
        }
    }
    std::fclose(f);
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

std::vector<Solar*> Tablero::solaresPorColor(const std::string& color) const {
    std::map<std::string, std::vector<Solar*> >::const_iterator it = grupos_.find(color);
    if (it != grupos_.end()) {
        return it->second;
    }
    return std::vector<Solar*>();
}

std::vector<Tablero::EstadoPropiedad> Tablero::capturarEstado(const std::vector<Jugador>& jugadores) const {
    std::vector<EstadoPropiedad> estados;
    for (std::size_t i = 0; i < casillas_.size(); ++i) {
        Propiedad* prop = dynamic_cast<Propiedad*>(casillas_[i]);
        if (prop) {
            EstadoPropiedad e;
            e.indice = static_cast<int>(i);
            e.hipotecada = prop->estaHipotecada();
            e.dueno = -1;
            if (prop->dueno()) {
                for (std::size_t j = 0; j < jugadores.size(); ++j) {
                    if (jugadores[j].nombre() == prop->dueno()->nombre()) {
                        e.dueno = static_cast<int>(j);
                        break;
                    }
                }
            }
            Solar* solar = dynamic_cast<Solar*>(prop);
            if (solar) {
                e.casas = solar->casas();
                e.hotel = solar->hotel();
            } else {
                e.casas = 0;
                e.hotel = false;
            }
            estados.push_back(e);
        }
    }
    return estados;
}

void Tablero::restaurarEstado(const std::vector<EstadoPropiedad>& estado, std::vector<Jugador>& jugadores) {
    for (std::size_t j = 0; j < jugadores.size(); ++j) {
        jugadores[j].limpiarPropiedades();
    }
    for (std::size_t i = 0; i < estado.size(); ++i) {
        const EstadoPropiedad& e = estado[i];
        if (static_cast<std::size_t>(e.indice) >= casillas_.size()) continue;
        Propiedad* prop = dynamic_cast<Propiedad*>(casillas_[static_cast<std::size_t>(e.indice)]);
        if (!prop) continue;
        prop->deshipotecar();
        if (e.hipotecada) prop->hipotecar();
        if (e.dueno >= 0 && static_cast<std::size_t>(e.dueno) < jugadores.size()) {
            prop->asignarDueno(&jugadores[static_cast<std::size_t>(e.dueno)]);
            jugadores[static_cast<std::size_t>(e.dueno)].agregarPropiedad(prop);
        } else {
            prop->asignarDueno(nullptr);
        }
        Solar* solar = dynamic_cast<Solar*>(prop);
        if (solar) {
            solar->fijarEdificaciones(e.casas, e.hotel);
        }
    }
}

void Tablero::describirCasilla(int indice) const {
    Casilla* c = obtener(indice);
    if (!c) return;
    std::cout << "Posición " << indice << ": " << c->nombre();
    if (c->tipo() == Casilla::Tipo::SOLAR) {
        Solar* s = (Solar*)c;
        std::cout << " | Solar color " << s->color() << " precio $" << s->datos().precio();
    } else if (c->tipo() == Casilla::Tipo::FERROCARRIL) {
        Ferrocarril* f = (Ferrocarril*)c;
        std::cout << " | Ferrocarril precio $" << f->datos().precio();
    } else if (c->tipo() == Casilla::Tipo::SERVICIO) {
        Servicio* s = (Servicio*)c;
        std::cout << " | Servicio precio $" << s->datos().precio();
    } else {
        Especial* e = (Especial*)c;
        std::cout << " | Especial";
        if (e->tipoEspecial() == Especial::IMPUESTO) {
            std::cout << " (impuesto $" << e->montoImpuesto() << ")";
        }
    }
    std::cout << "\n";
}

}

