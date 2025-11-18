#ifndef REGLAS_H
#define REGLAS_H

#include "Banco.h"
#include "Estado.h"
#include "../modelo/Jugador.h"
#include "../modelo/Casilla.h"
#include "../modelo/Especial.h"

/**
 * @class Reglas
 * @brief Procesa reglas generales y parámetros configurables del juego.
 */
class Reglas {
public:
    Reglas();

    /**
     * @brief Carga configuración desde un archivo de texto simple.
     * @param ruta Ruta del archivo.
     */
    void cargarConfig(const std::string& ruta);

    /**
     * @brief Evalúa el efecto de una casilla especial.
     */
    void evaluarCasilla(modelo::Especial* especial, modelo::Jugador& jugador, Banco& banco, int posicionCarcel) const;

    int salarioGo() const { return salarioGO; }
    int multaCarcel() const { return multaCarcelConfig; }
    int stockCasasInicial() const { return stockCasas; }
    int stockHotelesInicial() const { return stockHoteles; }
    int interesHipoteca() const { return interesHipotecaPorc; }

private:
    int salarioGO;
    int multaCarcelConfig;
    int stockCasas;
    int stockHoteles;
    int interesHipotecaPorc;
};

#endif
