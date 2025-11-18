#ifndef DADO_H
#define DADO_H

/**
 * @class Dado
 * @brief Representa un dado de seis caras.
 */
class Dado {
public:
    /**
     * @brief Lanza el dado.
     * @pre srand() debe ser llamado al inicio del programa para mejorar aleatoriedad.
     * @post Retorna un número entre 1 y 6 sin modificar estado interno.
     */
    int lanzar();
};

#endif
