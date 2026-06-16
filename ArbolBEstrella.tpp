#include <iostream>
#include <new>

// Métodos públicos

//Constructor
template<typename T>
ArbolBEstrella<T>::ArbolBEstrella(unsigned int orden) : orden_(orden), max_claves_(orden - 1), raiz_(nullptr) {}

template<typename T>
ArbolBEstrella<T>::ArbolBEstrella(const ArbolBEstrella &arbol) : orden_(arbol.orden_), max_claves_(arbol.orden_ - 1), raiz_(nullptr) {
    *this = arbol;
}

template<typename T>
ArbolBEstrella<T>& ArbolBEstrella<T>::operator=(const ArbolBEstrella &arbol) {
    if (this == &arbol) return *this;

    destruir(this->raiz_);
    this->raiz_ = nullptr;

    this->orden_ = arbol.orden_;
    this->max_claves_ = arbol.max_claves_;
    this->raiz_ = copiarNodo(arbol.raiz_);

    return *this;
}


//Buscar Público
template<typename T>
bool ArbolBEstrella<T>::buscar(T clave) const {
    Nodo<T> *auxiliar = nullptr;
    int i = 0;
    return buscar(raiz_, clave, auxiliar, i);
}

// Destructor
template<typename T>
ArbolBEstrella<T>::~ArbolBEstrella() {
    destruir(raiz_);
}

//============================
// Métodos privados
//============================

//Método Destruir Privado
template<typename T>
void ArbolBEstrella<T>::destruir(Nodo<T>* nodo) {
    if (nodo == nullptr) return;
    for (int i = 0; i < nodo->num_claves_+ 1; ++i) {
        destruir(nodo->hijos_[i]); //eliminamos los hijos antes que los padres
    }
    delete nodo;
}

//Buscar privado
template<typename T>
bool ArbolBEstrella<T>::buscar(Nodo<T> *subraiz, T clave, Nodo<T>* &auxiliar, int &indice) const{
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

template<typename T>
Nodo<T>* ArbolBEstrella<T>::copiarNodo(Nodo<T> *nodo) {
    if (nodo == nullptr) return nullptr;

    Nodo<T> *nuevo = nullptr;
    try {
        nuevo = new Nodo<T>(this->orden_, nodo->es_hoja_);
    }
    catch (const std::bad_alloc& mensaje) {
        std::cerr << "Falla de alojamiento de memoria:" << mensaje.what();
        return nullptr;
    }
    nuevo->num_claves_ = nodo->num_claves_;

    for (int i = 0; i < nodo->num_claves_; i++) {
        nuevo->claves_[i] = nodo->claves_[i];
    }

    for (int i = 0; i < nuevo->num_claves_ + 1; i++) {
        nuevo->hijos_[i] = copiarNodo(nodo->hijos_[i]);

        if (nuevo->hijos_[i] != nullptr) {
            nuevo->hijos_[i]->padre_ = nuevo;
        }
    }

    return nuevo;
}
