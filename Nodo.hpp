#ifndef ARBOLB_NODO_HPP
#define ARBOLB_NODO_HPP


// Implementación de la estructura Nodo
template <typename T>
struct Nodo {

    Nodo(unsigned int orden, bool es_hoja); //Constructor
    ~Nodo(); //Destructor
    Nodo<T>& operator=(const Nodo<T> &nodo);

    T* claves_;
    Nodo<T> *padre_;
    Nodo<T> **hijos_;
    int num_claves_;
    bool es_hoja_;


};

#include "Nodo.tpp"

#endif //ARBOLB_NODO_HPP