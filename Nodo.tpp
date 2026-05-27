//Constructor
template<typename T>
Nodo<T>::Nodo(unsigned int orden, bool es_hoja){
    int max_claves = orden - 1;

    this->claves_ = new T[max_claves + 1]; // Espacio extra para el overflow de claves
    this->hijos_ = new Nodo *[orden + 1]; // Espacio extra para el overflow de claves

    this-> num_claves_ = 0;
    this->es_hoja_ = es_hoja;

    for (int i = 0; i < orden + 1; i++) {
        hijos_[i] = nullptr;
    }

}
//Destructor
template<typename T>
Nodo<T>::~Nodo() {
    delete[] claves_;
    delete[] hijos_;
}

