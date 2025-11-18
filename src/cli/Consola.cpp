#include "Consola.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include "../juego/Juego.h" // ajustar ruta según el proyecto

Consola::Consola() : activo(false) {}

void Consola::inicializar(Juego* juego) {
    if (!juego) return;
    registrarComandosBase(juego);
    activo = true;
}

void Consola::registrarComandosBase(Juego* juego) {
    // Comando: iniciar (ya se asume que juego->inicializarJugadores existe)
    comandos.registrar("iniciar", [juego](const std::string& args){
        juego->inicializarJugadores();
    });

    // Comando: turno -> juega un turno
    comandos.registrar("turno", [juego](const std::string& args){
        juego->jugarTurno();
    });

    // Comando: estado -> muestra estado
    comandos.registrar("estado", [juego](const std::string& args){
        juego->mostrarEstado();
    });

    // Comando: jugar (loop hasta terminar una partida completa)
    comandos.registrar("jugar", [juego](const std::string& args){
        while (!juego->haTerminado()) {
            juego->jugarTurno();
        }
        std::cout << "Juego finalizado.\n";
    });

    // Comando: ayuda
    comandos.registrar("ayuda", [this](const std::string& args){
        std::cout << "Comandos disponibles:\n";
        for (const auto& p : comandos.listar()) {
            std::cout << " - " << p.first << "\n";
        }
    });

    // Comando: salir
    comandos.registrar("salir", [this](const std::string& args){
        std::cout << "Saliendo...\n";
        this->activo = false;
    });

    // Comando: construir <propiedad>
    comandos.registrar("construir", [juego](const std::string& args){
        // args => nombre de la propiedad
        // supondremos que existe un manejador en Juego para construir
        // ADAPTAR: juego->construir(args);
        // Si no existe, se puede invocar reglas/Construccion desde aquí.
        std::cout << "Solicitado construir en: " << args << "\n";
        // Intentamos método en Juego (si existe)
        // ADAPTAR: si no lo tienes, conecta con Construccion.cpp
        // ejemplo:
        // juego->construir(args);
    });

    // Comando: hipotecar <propiedad>
    comandos.registrar("hipotecar", [juego](const std::string& args){
        std::cout << "Solicitado hipotecar: " << args << "\n";
        // ADAPTAR: juego->hipotecar(args);
    });

    // Comando: comprar <propiedad>
    comandos.registrar("comprar", [juego](const std::string& args){
        std::cout << "Solicitado comprar: " << args << "\n";
        // ADAPTAR: juego->comprarPropiedad(args);
    });
}

void Consola::run() {
    if (!activo) {
        std::cerr << "Consola no inicializada. Llama a inicializar(juego) antes.\n";
        return;
    }
    std::string linea;
    std::cout << "Bienvenido al Monopoly CLI. Escribe 'ayuda' para ver comandos.\n";
    while (activo) {
        std::cout << "> ";
        if (!std::getline(std::cin, linea)) break;
        // trim
        linea.erase(linea.find_last_not_of(" \n\r\t")+1);
        if (linea.empty()) continue;
        if (!comandos.ejecutar(linea)) {
            std::cout << "Comando no reconocido. Escribe 'ayuda'.\n";
        }
    }
}
