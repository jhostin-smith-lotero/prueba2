#pragma once

#include <string>
#include <vector>

namespace juego {
class Juego;
}

namespace cli {

class Consola {
public:
    void mostrarBienvenida() const;
    void mostrarEstado(const juego::Juego& juego) const;
    void mostrarMensaje(const std::string& mensaje) const;

    int mostrarMenu(const std::string& titulo, const std::vector<std::string>& opciones) const;
    int solicitarEntero(const std::string& prompt, int minimo, int maximo) const;
    std::vector<std::string> solicitarNombresJugadores() const;
    std::string leerLinea(const std::string& prompt) const;

private:
    int leerEntero() const;
};

} // namespace cli
