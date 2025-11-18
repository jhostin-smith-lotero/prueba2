#ifndef COLA_H
#define COLA_H
#include <cstddef>

namespace edd {

template <typename T>
struct Cola {
public:
    Cola() : frente_(nullptr), fondo_(nullptr), tam_(0) {}

    Cola(const Cola& other) : frente_(nullptr), fondo_(nullptr), tam_(0) {
        copiarDesde(other);
    }

    Cola& operator=(const Cola& other) {
        if (this != &other) {
            clear();
            copiarDesde(other);
        }
        return *this;
    }

    ~Cola() {
        clear();
    }

    void enqueue(const T& valor) {
        Nodo* nuevo = new Nodo();
        nuevo->valor = valor;
        nuevo->siguiente = nullptr;
        enlazarNuevo(nuevo);
    }

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

    bool vacia() const {
        return frente_ == nullptr;
    }

    std::size_t tam() const {
        return tam_;
    }

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

    Nodo* frente_;
    Nodo* fondo_;
    std::size_t tam_;
};

}
#endif //COLA_H
