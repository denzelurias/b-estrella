#ifndef B_ESTRELLA_ARBOLBESTRELLA_HPP
#define B_ESTRELLA_ARBOLBESTRELLA_HPP

#include "Nodo.hpp"
#include <exception>

template <typename T>
class ArbolBEstrella {
public:
    ArbolBEstrella(unsigned int orden);
    ArbolBEstrella(const ArbolBEstrella &arbol);
    ArbolBEstrella& operator=(const ArbolBEstrella &arbol);
    ~ArbolBEstrella();

    bool buscar(T clave) const; //
    bool insertar(T clave); // Hecho por Marco en pseudocodigo
    void imprimir() const; // Ojaldrez
    bool eliminar(T clave);
    void vaciar();

    //Funcion eliminar - Ojaldrez
    // Operador igual - Denzel

    //Clase anidada para señalar fallos de reserva de memoria
    class MemoriaInsuficiente : public std::exception {
    public:
        MemoriaInsuficiente() throw();
        virtual const char* what() const throw();
    };




private:
    int orden_;
    int max_claves_;
    Nodo<T> *raiz_;
    bool buscar(Nodo<T> *subraiz, T clave, Nodo<T>* &auxiliar, int &indice) const;
    void destruir(Nodo<T>* nodo);
    Nodo<T>* copiarNodo(Nodo<T> *nodo);
    void redistribuir(Nodo<T> *nodo_padre, Nodo<T> *nodo_hijo_izq, Nodo<T> *nodo_hijo_der, int indice_nodo_hijo_izq);
    void dividir(Nodo<T> *nodo_padre, Nodo<T> *nodo_hijo_izq, Nodo<T> *nodo_hijo_der, int indice_nodo_hijo_izq);
    void fusionar(Nodo<T> *padre, int indice);
    void dividirRaiz(Nodo<T> *vieja_raiz);
    int obtenerIndice(Nodo<T> **arreglo_hijos, int num_hijos, Nodo<T> *nodo_buscado) const;
    int insertarEnOrden(T arreglo[], int num_elem, const T &elem_insertar);
    void insertarEnIndice(Nodo<T> **arreglo_nodos, int num_nodos, Nodo<T> *nodo_insertar, int indice_insercion);

    void imprimir(Nodo<T> *nodo, int nivel) const;
    bool eliminar(Nodo<T> *subraiz, T clave);
    T obtenerSucesor(Nodo<T> *nodo, int indice) const;
    T obtenerPredecesor(Nodo<T> *nodo, int indice) const;
    bool esDeficiente(Nodo<T> *nodo) const;
    void corregirDeficiencia(Nodo<T> *nodo_padre, int indice_hijo);

    int minimoClaves() const;
    void ajustarRaizDespuesDeEliminar();
    void fusionarDosHijos(Nodo<T> *nodo_padre, int indice_hijo_izq);




};

#include "ArbolBEstrella.tpp"

#endif //B_ESTRELLA_ARBOLBESTRELLA_HPP
