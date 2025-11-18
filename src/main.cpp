#include "cli/Consola.h"
#include "juego/Juego.h"

int main() {
    Juego juego;
    Consola consola;
    consola.inicializar(&juego);
    consola.run();
    return 0;
}
