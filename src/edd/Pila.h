#ifndef PILA_H
#define PILA_H

#include <cstddef>
#include <vector>

namespace edd {

template <typename T>
struct Pila {
public:
    Pila() : cima_(nullptr), tam_(0) {}

    Pila(const Pila& other) : cima_(nullptr), tam_(0) {
        copiarDesde(other);
    }

    Pila& operator=(const Pila& other) {
        if (this != &other) {
            limpiar();
            copiarDesde(other);
        }
        return *this;
    }

    ~Pila() {
        limpiar();
    }

    void apilar(const T& valor) {
        Nodo* nuevo = new Nodo();
        nuevo->valor = valor;
        nuevo->siguiente = cima_;
        cima_ = nuevo;
        ++tam_;
    }

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

    bool vacia() const {
        return cima_ == nullptr;
    }

    void limpiar() {
        while (cima_) {
            Nodo* actual = cima_;
            cima_ = actual->siguiente;
            delete actual;
        }
        tam_ = 0;
    }

    std::size_t tam() const {
        return tam_;
    }

private:
    struct Nodo {
        T valor;
        Nodo* siguiente;
    };

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

    Nodo* cima_;
    std::size_t tam_;
};

}
#endif //PILA_H
