/**
 * @file cola.tpp
 * @brief Implementaci&oacute;n de los m&eacute;todos de la plantilla cola&lt;T&gt;.
 *
 * Este archivo se incluye al final de @c cola.hpp porque las plantillas
 * necesitan que la definici&oacute;n est&eacute; visible donde se instancien.
 *
 * @author Denzel Omar Urias Rivera, Angel David Ortega Felix
 * @date 06/03/2026
 */

#include <iostream>

// ============================================================================
//  Constructor, destructor y regla de los tres
// ============================================================================

template <typename T>
cola<T>::cola() : _numElem(0), _fondo(nullptr) {}

template <typename T>
cola<T>::~cola() { vaciar(); }

template <typename T>
cola<T>::cola(const cola<T> &s) : _numElem(0), _fondo(nullptr) {
    *this = s;
}

template <typename T>
cola<T> &cola<T>::operator=(const cola<T> &c) {
    if (this == &c) return *this;
    vaciar();

    if (c.estaVacia()) return *this;

    // Recorremos la cola original desde el frente hasta el fondo
    elemento *visitado = c._fondo->_siguiente;
    int copiados = 0;
    while (copiados < c._numElem) {
        agregar(visitado->_valor);
        visitado = visitado->_siguiente;
        ++copiados;
    }
    return *this;
}

// ============================================================================
//  Operaciones principales
// ============================================================================

template <typename T>
void cola<T>::agregar(T valor) {
    try {
        elemento *nuevo = new elemento(valor);

        if (estaVacia())
            nuevo->_siguiente = nuevo; // Apunta a sí mismo
        else
            nuevo->_siguiente = _fondo->_siguiente; // El nuevo fondo apunta al frente

        if (!estaVacia())
            _fondo->_siguiente = nuevo; // El viejo fondo enlaza al nuevo

        _fondo = nuevo;
        ++_numElem;

    } catch (std::bad_alloc &) {
        throw std::runtime_error("No es posible agregar el elemento a la cola");
    }
}

template <typename T>
void cola<T>::eliminar() {
    if (estaVacia())
        throw std::runtime_error("La cola está vacía");

    elemento *porBorrar = _fondo->_siguiente; // El frente es el que sale

    if (_numElem == 1)
        _fondo = nullptr;        // Se queda vacía
    else
        _fondo->_siguiente = porBorrar->_siguiente; // Saltamos al siguiente frente

    delete porBorrar;
    --_numElem;
}

template <typename T>
void cola<T>::vaciar() {
    while (!estaVacia())
        eliminar();
}

// ============================================================================
//  Consultas
// ============================================================================

template <typename T>
T cola<T>::frente() const {
    if (estaVacia())
        throw std::runtime_error("Error: La cola está vacía.");

    return _fondo->_siguiente->_valor;
}

template <typename T>
T cola<T>::fondo() const {
    if (estaVacia())
        throw std::runtime_error("Error: La cola está vacía.");

    return _fondo->_valor;
}

template <typename T>
int cola<T>::tamano() const { return _numElem; }

template <typename T>
bool cola<T>::estaVacia() const { return _numElem == 0; }

// ============================================================================
//  Impresión
// ============================================================================

template <typename T>
void cola<T>::imprimir() const {
    if (estaVacia())
        throw std::runtime_error("La cola está vacía");

    // Recorremos desde el frente hasta el fondo
    elemento *aux = _fondo->_siguiente;
    int impresos = 0;
    while (impresos < _numElem) {
        std::cout << aux->_valor << std::endl;
        aux = aux->_siguiente;
        ++impresos;
    }
}

// ============================================================================
//  Nodo interno
// ============================================================================

template <typename T>
cola<T>::elemento::elemento(T valor, cola<T>::elemento *siguiente)
    : _valor(valor), _siguiente(siguiente) {}
