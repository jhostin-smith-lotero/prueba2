#include "cli/Comandos.h"
#include "cli/Consola.h"
#include "juego/Juego.h"

#include <exception>
#include <iostream>

int main() {
    cli::Consola consola;
    cli::Comandos comandos;
    juego::Juego juego;

    try {
        juego.cargarDatos("data");
    } catch (const std::exception& e) {
        std::cerr << "Error cargando datos: " << e.what() << std::endl;
        return 1;
    }

    consola.mostrarBienvenida();
    bool continuar = true;
    while (continuar) {
        continuar = comandos.procesar(juego, consola);
    }
    consola.mostrarMensaje("Hasta pronto");
    return 0;
}
