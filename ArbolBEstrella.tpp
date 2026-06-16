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

//Función privada fusionar
template <typename T>
void ArbolBEstrella<T>:: fusionar(Nodo<T> *padre, int indice) {

    //Creamos nodos para trabajar más fácil con los hijos
    Nodo<T> *izq = padre->hijos_[indice];
    Nodo<T> *medio = padre->hijos_[indice +1];
    Nodo<T> *der = padre->hijos_[indice+2];

    // Guardamos todas las claves(del padre y de los hijos) en un arreglo epicamente
    T temp [3 * orden_ - 1]; //Formula para saber el tamaño del arreglo epica
    int pos = 0; //Variable para saber la pos de las claves que guardamos

    //Guardamos todas las claves del lado izquierdo
    for (int i = 0; i < izq->num_claves_; ++i) {
        temp[pos++] = izq->claves_[i];
    }

    //Guardamos la clave del padre
    temp[pos++] = padre->claves_[indice];

    //Guardamos las claves de en medio
    for (int i = 0; i < medio->num_claves_; ++i) {
        temp[pos++] = medio->claves_[i];
    }

    //Guardamos la segunda clave separadora del padre
    temp[pos++] = padre->claves_[indice + 1];

    //Guardamos las claves del hijo del lado derecho
    for (int i = 0;  i < der->num_claves_; ++i) {
        temp[pos++] = der->claves_[i];
    }

    //Parte 2 epicamente identifico la clave del arreglo para mover  y sea el nuevo padre
    padre->claves_[indice] = temp[pos/2];

    //Actualizamos las claves del padre
    for (int i = indice + 1; i < padre->num_claves_ -1; ++i) {
        padre->claves_[i] = padre->claves_[i+1];
    }

    //Recorremos los punteros hijos del padre hacia la izquierda
    for (int i = indice + 2; i< padre->num_claves_; ++i ) {
        padre ->hijos_[i] = padre -> hijos_[i+1];
    }
    //Reducimos el número de claves
    padre->num_claves_--;

    //Limpiamos el puntero colgante del hijo eliminado
    padre->hijos_[padre ->num_claves_ + 1] = nullptr;

    //mandamos al lado izquierdo las claves que estamos reorganizando
    for (int i = 0; i < pos/2 ; ++i) {
        izq->claves_[i] = temp[i];
    }

    //Actualizamos cuantas claves tiene el lado izq
    izq->num_claves_ = pos/2;
    int j = 0; // Variable para que sepa la pos de las claves el nodo de en medio
    for (int i = pos/2 + 1; i < pos; ++i) {
        medio->claves_[j++] = temp[i];
    }

    medio->num_claves_ = j; //ahora con j sabemos cuantas claves tiene el nodo de en medio

    // Actualizamos los hijos
    //Basicamente lo mismo que lo anterior pero con los hijos
    Nodo<T> *temp_hijos [3 * orden_]; //El tamaño del arreglo es 1 más
    pos = 0;
    j = 0;

    // Metemos todos los punteros a hijos en un arreglo

    // todo del lado izquierdo
    for (int i = 0; i <= izq->num_claves_; ++i) {
        temp_hijos[pos++] = izq->hijos_[i];
    }

    // todo de en medio
    for (int i = 0; i <= medio->num_claves_; ++i) {
        temp_hijos[pos++] = medio->hijos_[i];
    }

    // todo del lado derecho
    for (int i = 0; i <= der->num_claves_; ++i) {
        temp_hijos[pos++] = der->hijos_[i];
    }

    // Redistribuimos los punteros a hijos

    //Primero del lado izquierdo
    for (int i = 0; i <= izq->num_claves_; ++i) {
        izq->hijos_[i] = temp_hijos[i];
        // Les decimos que tienen nuevo padre a cada hijo del lado izquierdo
        if (temp_hijos[i] != nullptr) {
            temp_hijos[i]->padre_ = izq;
        }
    }

    int k = 0;
    for (int i = izq->num_claves_ + 1; i < pos; ++i ) {
        medio->hijos_[k++] = temp_hijos[i];
        //Les decimos que tienen nuevo padre a cada hijo del lado de en medio
        if (temp_hijos[i] != nullptr) {
            temp_hijos[i]->padre_ = medio;
        }
    }

    //Borramos el lado derecho
    delete der;


}