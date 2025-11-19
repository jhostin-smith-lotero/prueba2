#ifndef PILA_H
#define PILA_H

#include <cstddef>
#include <vector>

namespace edd {

/**
 * @class Pila
 * @brief Implementación de una pila LIFO usando nodos enlazados.
 * @tparam T Tipo almacenado.
 */
template <typename T>
struct Pila {
public:
    /**
     * @brief Constructor por defecto.
     * @pre Ninguna.
     * @post Crea una pila vacía.
     */
    Pila() : cima_(nullptr), tam_(0) {}

    /**
     * @brief Constructor copia.
     * @pre Ninguna.
     * @post La nueva pila contiene copias de los elementos de `other` en el mismo orden.
     */
    Pila(const Pila& other) : cima_(nullptr), tam_(0) {
        copiarDesde(other);
    }

    /**
     * @brief Operador asignación.
     * @pre Ninguna.
     * @post La pila actual contendrá los elementos de `other`.
     */
    Pila& operator=(const Pila& other) {
        if (this != &other) {
            limpiar();
            copiarDesde(other);
        }
        return *this;
    }

    /**
     * @brief Destructor.
     * @pre Ninguna.
     * @post Libera todos los nodos almacenados.
     */
    ~Pila() {
        limpiar();
    }

    /**
     * @brief Inserta un elemento en la cima.
     * @param valor Elemento a añadir.
     * @pre Ninguna.
     * @post El elemento queda como nuevo tope de la pila.
     */
    void apilar(const T& valor) {
        Nodo* nuevo = new Nodo();
        nuevo->valor = valor;
        nuevo->siguiente = cima_;
        cima_ = nuevo;
        ++tam_;
    }

    /**
     * @brief Extrae el elemento en la cima.
     * @param out Variable donde se guarda el valor extraído.
     * @pre La pila no debe estar vacía.
     * @post El valor del tope es eliminado y almacenado en `out`.
     * @return true si se extrajo con éxito.
     */
    bool desapilar(T& out) {
        if (!cima_) {
            return false;
        }
        Nodo* actual = cima_;
        out = actual->valor;
        cima_ = actual->siguiente;
        delete actual;
        --tam_;
        return true;
    }

    /**
     * @brief Indica si la pila está vacía.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    bool vacia() const {
        return cima_ == nullptr;
    }

    /**
     * @brief Elimina todos los elementos.
     * @pre Ninguna.
     * @post La pila queda vacía.
     */
    void limpiar() {
        while (cima_) {
            Nodo* actual = cima_;
            cima_ = actual->siguiente;
            delete actual;
        }
        tam_ = 0;
    }

    /**
     * @brief Obtiene la cantidad de elementos en la pila.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    std::size_t tam() const {
        return tam_;
    }

private:
    struct Nodo {
        T valor;
        Nodo* siguiente;
    };

    Nodo* cima_;
    std::size_t tam_;

    void copiarDesde(const Pila& other) {
        std::vector<const Nodo*> nodos;
        const Nodo* cursor = other.cima_;
        while (cursor) {
            nodos.push_back(cursor);
            cursor = cursor->siguiente;
        }
        for (std::size_t i = nodos.size(); i > 0; --i) {
            apilar(nodos[i - 1]->valor);
        }
    }
};

}

#endif // PILA_H
