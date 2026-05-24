#ifndef ARBOLB_NODO_HPP
#define ARBOLB_NODO_HPP


// Implementación de la estructura Nodo
template <typename T>
struct Nodo {
    Nodo();
    ~Nodo();

    T* claves_;
    Nodo** hijos_;
    int numClaves_;
    bool es_hoja_;
};

#include "Nodo.tpp"

#endif //ARBOLB_NODO_HPP