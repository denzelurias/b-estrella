// Métodos públicos
template<typename T>
ArbolBEstrella<T>::ArbolB(unsigned int orden) : orden_(orden), max_claves_(orden - 1), raiz_(nullptr) {}

template<typename T>
ArbolBEstrella<T>::buscar(T clave) {
    Nodo<T> *auxiliar;
    int i = 0;
    return buscar(raiz_, clave, auxiliar, i);
}

// Métodos privados
template<typename T>
bool ArbolBEstrella<T>::buscar(Nodo<T> *subraiz, T clave, Nodo<T>* &auxiliar, int &indice) {
    if (subraiz == nullptr) {
        return false;
    }
    int i = 0;
    while (i < subraiz->num_claves_ && clave > subraiz->claves_[i]) {
        i++;
    }

    auxiliar = subraiz;
    indice = i;

    if (i < subraiz->num_claves_ && clave == subraiz->claves_[i]) {
        return true;
    }

    if (subraiz->es_hoja_ == true) {
        return false;
    }

    return buscar(subraiz->hijos_[i], clave, auxiliar, indice);
}
