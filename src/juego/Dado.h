#ifndef DADO_H
#define DADO_H

/**
 * @class Dado
 * @brief Representa un dado estándar de 6 caras.
 */
class Dado {
public:
    /**
     * @brief Lanza el dado.
     * @pre srand() debe haberse llamado en el inicio del programa.
     * @post Retorna un número aleatorio entre 1 y 6.
     */
    int lanzar();
};

#endif
