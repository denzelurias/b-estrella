/**
* @file Nodo.hpp
 * @brief Declaración de la plantilla de estructura Nodo<> para un árbol B*.
 *
 * Cada nodo almacena arreglos dinámicos de claves e hijos, un apuntador a su
 * padre, la cantidad actual de claves y una marca para indicar si es hoja.
 *
 * @date 18/06/2026
 */

#ifndef ARBOLB_NODO_HPP
#define ARBOLB_NODO_HPP

/**
 * \brief Nodo genérico usado por ArbolBEstrella<>.
 *
 * La estructura contiene únicamente la información local de un nodo. La
 * destrucción recursiva del árbol completo se realiza desde ArbolBEstrella,
 * no desde el destructor de Nodo.
 *
 * \tparam T Tipo de dato almacenado como clave.
 */
template <typename T>
struct Nodo {
    /**
     * \brief Constructor con orden y tipo de nodo.
     *
     * Reserva espacio para claves e hijos. Se deja una posición extra para
     * manejar temporalmente overflow durante operaciones de inserción.
     *
     * \param orden unsigned int Orden del árbol al que pertenece el nodo.
     * \param es_hoja bool true si el nodo es hoja, false si es interno.
     */
    Nodo(unsigned int orden, bool es_hoja);

    /**
     * \brief Destructor.
     *
     * Libera los arreglos internos de claves e hijos. No elimina los nodos hijos;
     * esa responsabilidad pertenece al árbol.
     */
    ~Nodo();

    /**
     * \brief Operador de asignación.
     * \param nodo const Nodo<T>& Nodo origen.
     * \return Nodo<T>& Referencia al nodo actual.
     */
    Nodo<T>& operator=(const Nodo<T> &nodo);

    T* claves_;        ///< Arreglo dinámico de claves almacenadas en orden.
    Nodo<T> *padre_;   ///< Apuntador al padre del nodo; nullptr si es raíz.
    Nodo<T> **hijos_;  ///< Arreglo dinámico de apuntadores a hijos.
    int num_claves_;   ///< Número actual de claves válidas en el nodo.
    bool es_hoja_;     ///< Indica si el nodo no tiene hijos activos.
};

#include "Nodo.tpp"

#endif //ARBOLB_NODO_HPP
