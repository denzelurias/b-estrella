#ifndef B_ESTRELLA_ARBOLBESTRELLA_HPP
#define B_ESTRELLA_ARBOLBESTRELLA_HPP

#include "Nodo.hpp"

template <typename T>
class ArbolBEstrella {
public:
    ArbolBEstrella(unsigned int orden);
    ~ArbolBEstrella();

    bool buscar(T clave) const;
    bool insertar(T clave);
    void imprimir() const;

private:
    int orden_;
    int max_claves_;
    Nodo<T> *raiz_;

    bool buscar(Nodo<T> *subraiz, T clave, Nodo<T>& *auxiliar = nullptr, int &indice = 0);
};

#include "ArbolBEstrella.tpp"

#endif //B_ESTRELLA_ARBOLBESTRELLA_HPP