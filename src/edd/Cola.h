#ifndef COLA_H
#define COLA_H
#include <cstddef>

namespace edd {

/**
 * @class Cola
 * @brief Implementación simple de una cola FIFO enlazada.
 * @tparam T Tipo de los elementos almacenados.
 */
template <typename T>
struct Cola {
public:
    /**
     * @brief Constructor por defecto.
     * @pre Ninguna.
     * @post Crea una cola vacía.
     */
    Cola() : frente_(nullptr), fondo_(nullptr), tam_(0) {}

    /**
     * @brief Constructor copia.
     * @pre Ninguna.
     * @post La cola se inicializa conteniendo una copia de los elementos de `other`.
     */
    Cola(const Cola& other) : frente_(nullptr), fondo_(nullptr), tam_(0) {
        copiarDesde(other);
    }

    /**
     * @brief Operador asignación.
     * @pre Ninguna.
     * @post La cola actual contendrá una copia de `other`.
     *       La cola previa es liberada correctamente.
     */
    Cola& operator=(const Cola& other) {
        if (this != &other) {
            clear();
            copiarDesde(other);
        }
        return *this;
    }

    /**
     * @brief Destructor.
     * @pre Ninguna.
     * @post Libera todos los nodos de la cola.
     */
    ~Cola() {
        clear();
    }

    /**
     * @brief Inserta un valor al final de la cola.
     * @param valor Elemento a agregar.
     * @pre Ninguna.
     * @post El elemento queda en el fondo de la cola.
     */
    void enqueue(const T& valor) {
        Nodo* nuevo = new Nodo();
        nuevo->valor = valor;
        nuevo->siguiente = nullptr;
        enlazarNuevo(nuevo);
    }

    /**
     * @brief Extrae un elemento del frente.
     * @param out Variable donde se almacena el elemento extraído.
     * @pre La cola no debe estar vacía.
     * @post El primer elemento se elimina y `out` recibe el valor.
     * @return true si se extrajo correctamente, false si la cola estaba vacía.
     */
    bool dequeue(T& out) {
        if (!frente_) {
            return false;
        }
        Nodo* actual = frente_;
        out = actual->valor;
        frente_ = actual->siguiente;
        if (!frente_) {
            fondo_ = nullptr;
        }
        delete actual;
        --tam_;
        return true;
    }

    /**
     * @brief Indica si la cola está vacía.
     * @pre Ninguna.
     * @post No modifica estado.
     * @return true si no contiene elementos.
     */
    bool vacia() const {
        return frente_ == nullptr;
    }

    /**
     * @brief Retorna el tamaño actual.
     * @pre Ninguna.
     * @post No modifica estado.
     */
    std::size_t tam() const {
        return tam_;
    }

    /**
     * @brief Vacía completamente la cola.
     * @pre Ninguna.
     * @post Todos los nodos son eliminados.
     */
    void clear() {
        while (frente_) {
            Nodo* actual = frente_;
            frente_ = actual->siguiente;
            delete actual;
        }
        fondo_ = nullptr;
        tam_ = 0;
    }

private:
    struct Nodo {
        T valor;
        Nodo* siguiente;
    };

    Nodo* frente_;
    Nodo* fondo_;
    std::size_t tam_;

    void enlazarNuevo(Nodo* nodo) {
        if (!fondo_) {
            frente_ = nodo;
        } else {
            fondo_->siguiente = nodo;
        }
        fondo_ = nodo;
        ++tam_;
    }

    void copiarDesde(const Cola& other) {
        const Nodo* cursor = other.frente_;
        while (cursor) {
            enqueue(cursor->valor);
            cursor = cursor->siguiente;
        }
    }
};

}
#endif // COLA_H
