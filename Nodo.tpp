/**
* @file Nodo.tpp
 * @brief Implementación de la plantilla de estructura Nodo<>.
 *
 * Define la construcción y destrucción de los arreglos dinámicos internos usados
 * por cada nodo del árbol B*.
 *
 * @date 18/06/2026
 */

// ============================================================================
//  Construcción y destrucción
// ============================================================================

/** \brief Construye un nodo y reserva sus arreglos internos. */
template<typename T>
Nodo<T>::Nodo(unsigned int orden, bool es_hoja){
    int max_claves = orden - 1;

    this->claves_ = new T[max_claves + 1]; // Espacio extra para el overflow de claves
    this->padre_ = nullptr;
    this->hijos_ = new Nodo<T> *[orden + 1]; // Espacio extra para el overflow de claves

    this-> num_claves_ = 0;
    this->es_hoja_ = es_hoja;

    for (int i = 0; i < orden + 1; i++) {
        hijos_[i] = nullptr;
    }

}

// ------------------------------------------------------- //

/** \brief Libera los arreglos internos del nodo. */
template<typename T>
Nodo<T>::~Nodo() {
    delete[] claves_;
    delete[] hijos_;
}
