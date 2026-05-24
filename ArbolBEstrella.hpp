#ifndef B_ESTRELLA_ARBOLBESTRELLA_TPP
#define B_ESTRELLA_ARBOLBESTRELLA_TPP

#include "Nodo.hpp"

template <typename T>
class ArbolBEstrella {
public:
    ArbolBEstrella(unsigned int orden);
    ~ArbolBEstrella();

private:
    int orden_;
    int max_claves_;
    Nodo<T> *raiz_;
};

#include "ArbolBEstrella.tpp"

#endif //B_ESTRELLA_ARBOLBESTRELLA_TPP