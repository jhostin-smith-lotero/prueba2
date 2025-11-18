#ifndef COMANDOS_H
#define COMANDOS_H

#include <string>
#include <vector>

class Juego;

class Comandos {
public:
    enum Accion {
        Tirar,
        Comprar,
        Construir,
        Hipotecar,
        Deshipotecar,
        Estado,
        Undo,
        Pasar,
        UsarCarta,
        PagarMulta
    };

    struct EntradaMenu {
        int numero;
        Accion accion;
        std::string texto;
    };

    Comandos();

    void limpiar();
    void agregar(const EntradaMenu& entrada);
    bool ejecutar(int numero, Juego& juego);
    const std::vector<EntradaMenu>& opciones() const;

private:
    std::vector<EntradaMenu> lista;
};

#endif // COMANDOS_H
