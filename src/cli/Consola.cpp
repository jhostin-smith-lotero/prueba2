#include "Consola.h"

#include "../juego/Juego.h"
#include "../modelo/Jugador.h"

#include <iostream>
#include <limits>
#include <sstream>

namespace cli {

void Consola::mostrarBienvenida() const {
    std::cout << "Bienvenido a Mono'Terminal" << std::endl;
    std::cout << "Utiliza las opciones numeradas para jugar." << std::endl;
}

void Consola::mostrarEstado(const juego::Juego& juego) const {
    std::cout << juego.resumenEstado();
}

void Consola::mostrarMensaje(const std::string& mensaje) const {
    std::cout << mensaje << std::endl;
}

int Consola::mostrarMenu(const std::string& titulo, const std::vector<std::string>& opciones) const {
    if (!titulo.empty()) {
        std::cout << titulo << std::endl;
    }
    for (std::size_t i = 0; i < opciones.size(); ++i) {
        std::cout << (i + 1) << ". " << opciones[i] << std::endl;
    }
    while (true) {
        std::cout << "> Selecciona una opción: ";
        int seleccion = leerEntero();
        if (seleccion >= 1 && seleccion <= static_cast<int>(opciones.size())) {
            return seleccion - 1;
        }
        std::cout << "Opción inválida. Inténtalo nuevamente." << std::endl;
    }
}

int Consola::solicitarEntero(const std::string& prompt, int minimo, int maximo) const {
    while (true) {
        std::cout << prompt;
        int valor = leerEntero();
        if (valor >= minimo && valor <= maximo) {
            return valor;
        }
        std::cout << "Valor fuera de rango. Debe estar entre " << minimo << " y " << maximo << "." << std::endl;
    }
}

std::vector<std::string> Consola::solicitarNombresJugadores() const {
    std::cout << "Introduce los nombres de los jugadores separados por espacios: ";
    std::string linea;
    std::getline(std::cin, linea);
    std::istringstream iss(linea);
    std::vector<std::string> nombres;
    std::string nombre;
    while (iss >> nombre) {
        nombres.push_back(nombre);
    }
    return nombres;
}

std::string Consola::leerLinea(const std::string& prompt) const {
    std::cout << prompt;
    std::string linea;
    std::getline(std::cin, linea);
    return linea;
}

int Consola::leerEntero() const {
    int valor = 0;
    while (!(std::cin >> valor)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida. Inténtalo nuevamente: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return valor;
}

} // namespace cli
