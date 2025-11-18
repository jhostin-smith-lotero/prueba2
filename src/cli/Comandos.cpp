#include "Comandos.h"
#include <sstream>

Comandos::Comandos() {}

void Comandos::registrar(const std::string& nombre, std::function<void(const std::string&)> accion) {
    if (nombre.empty() || !accion) return;
    tabla[nombre] = accion;
}

bool Comandos::ejecutar(const std::string& linea) const {
    std::istringstream iss(linea);
    std::string cmd;
    iss >> cmd;
    if (cmd.empty()) return false;

    auto it = tabla.find(cmd);
    if (it == tabla.end()) return false;

    std::string resto;
    std::getline(iss, resto);
    // trim leading spaces
    if (!resto.empty() && resto.front() == ' ') resto.erase(0, 1);

    it->second(resto);
    return true;
}

std::map<std::string, std::function<void(const std::string&)>> Comandos::listar() const {
    return tabla;
}
