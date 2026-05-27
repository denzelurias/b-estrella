// Métodos públicos

//Constructor
template<typename T>
ArbolBEstrella<T>::ArbolBEstrella(unsigned int orden) : orden_(orden), max_claves_(orden - 1), raiz_(nullptr) {}

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

