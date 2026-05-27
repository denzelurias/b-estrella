#ifndef ARBOLB_NODO_HPP
#define ARBOLB_NODO_HPP


// Implementación de la estructura Nodo
template <typename T>
struct Nodo {

    Nodo(unsigned int orden, bool es_hoja); //Constructor
    ~Nodo(); //Destructor
    T* claves_;
    Nodo** hijos_;
    int num_claves_;
    bool es_hoja_;


};

#include "Nodo.tpp"

#endif //ARBOLB_NODO_HPP