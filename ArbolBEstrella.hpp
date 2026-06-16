#ifndef B_ESTRELLA_ARBOLBESTRELLA_HPP
#define B_ESTRELLA_ARBOLBESTRELLA_HPP

#include "Nodo.hpp"

template <typename T>
class ArbolBEstrella {
public:
    ArbolBEstrella(unsigned int orden);
    ArbolBEstrella(const ArbolBEstrella &arbol);
    ArbolBEstrella& operator=(const ArbolBEstrella &arbol);
    ~ArbolBEstrella();

    bool buscar(T clave) const; //
    bool insertar(T clave); // Hecho por Marco en pseudocodigo
    void imprimir() const; // Ojaldrez
    //Funcion eliminar - Ojaldrez
    // Operador igual - Denzel


private:
    int orden_;
    int max_claves_;
    Nodo<T> *raiz_;
    bool buscar(Nodo<T> *subraiz, T clave, Nodo<T>* &auxiliar, int &indice) const;
    void destruir(Nodo<T>* nodo);
    Nodo<T>* copiarNodo(Nodo<T> *nodo);
    //Función redistribuir
    //Función dividir
    void fusionar(Nodo<T> *padre, int indice);

};

#include "ArbolBEstrella.tpp"

#endif //B_ESTRELLA_ARBOLBESTRELLA_HPP