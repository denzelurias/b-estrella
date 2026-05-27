#ifndef B_ESTRELLA_ARBOLBESTRELLA_HPP
#define B_ESTRELLA_ARBOLBESTRELLA_HPP

#include "Nodo.hpp"

template <typename T>
class ArbolBEstrella {
public:
    ArbolBEstrella(unsigned int orden);
    ~ArbolBEstrella();
    bool buscar(T clave) const; //
    bool insertar(T clave); // Hecho por Marco en pseudocodigo
    void imprimir() const; // Que lo haga la ia
    //Funcion eliminar





private:
    int orden_;
    int max_claves_;
    Nodo<T> *raiz_;
    bool buscar(Nodo<T> *subraiz, T clave, Nodo<T>* &auxiliar, int &indice) const;
    void destruir(Nodo<T>* nodo);
    //Función redistribuir
    //Función dividir
    //
};

#include "ArbolBEstrella.tpp"

#endif //B_ESTRELLA_ARBOLBESTRELLA_HPP