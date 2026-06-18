/**
 * @file cola.hpp
 * @brief Declaraci&oacute;n de la plantilla de cola circular enlazada.
 *
 * Implementa una cola gen&eacute;rica (FIFO) usando una lista circular
 * simplemente enlazada donde @c _fondo apunta al &uacute;ltimo nodo insertado
 * y @c _fondo->_siguiente apunta al frente de la cola.
 *
 * @tparam T Tipo de dato almacenado en la cola (por defecto @c int).
 *
 * @author Denzel Omar Urias Rivera, Angel David Ortega Felix
 * @date 06/03/2026
 */

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>

template <typename T = int>
class cola {
public:
    /** @brief Construye una cola vac&iacute;a. */
    cola();

    /** @brief Constructor de copia. Duplica todos los elementos de @p p. */
    cola(const cola &p);

    /** @brief Operador de asignaci&oacute;n. Reemplaza el contenido con el de @p p. */
    cola &operator=(const cola &p);

    /** @brief Destructor. Libera toda la memoria de la cola. */
    ~cola();

    /**
     * @brief Agrega un elemento al fondo de la cola.
     * @param valor Elemento a insertar.
     * @throw std::runtime_error Si no se puede reservar memoria.
     */
    void agregar(T valor);

    /**
     * @brief Elimina el elemento del frente de la cola.
     * @throw std::runtime_error Si la cola est&aacute; vac&iacute;a.
     */
    void eliminar();

    /**
     * @brief Devuelve el elemento que est&aacute; al frente (el pr&oacute;ximo a salir).
     * @return Copia del elemento al frente.
     * @throw std::runtime_error Si la cola est&aacute; vac&iacute;a.
     */
    T frente() const;

    /**
     * @brief Devuelve el elemento que est&aacute; al fondo (el &uacute;ltimo insertado).
     * @return Copia del elemento al fondo.
     * @throw std::runtime_error Si la cola est&aacute; vac&iacute;a.
     */
    T fondo() const;

    /** @brief Elimina todos los elementos de la cola. */
    void vaciar();

    /**
     * @brief Devuelve la cantidad de elementos en la cola.
     * @return N&uacute;mero de elementos.
     */
    int tamano() const;

    /**
     * @brief Indica si la cola no tiene elementos.
     * @return @c true si est&aacute; vac&iacute;a, @c false en caso contrario.
     */
    bool estaVacia() const;

    /**
     * @brief Imprime todos los elementos de la cola desde el frente hasta el fondo.
     * @throw std::runtime_error Si la cola est&aacute; vac&iacute;a.
     */
    void imprimir() const;

private:
    int _numElem; ///< Cantidad actual de elementos en la cola.

    /**
     * @struct elemento
     * @brief Nodo interno de la lista circular que guarda un valor y un enlace.
     */
    struct elemento {
        /**
         * @brief Construye un nodo con un valor y un enlace opcional.
         * @param v     Valor almacenado.
         * @param next_ Puntero al siguiente nodo (nullptr por defecto).
         */
        elemento(T v, elemento *next_ = nullptr);

        T         _valor;     ///< Dato almacenado en este nodo.
        elemento *_siguiente; ///< Puntero al siguiente nodo en la lista circular.
    };

    elemento *_fondo; ///< Puntero al &uacute;ltimo nodo insertado (fondo de la cola).
};

#include "cola.tpp"

#endif // QUEUE_HPP
