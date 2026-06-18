/**
 * @file ArbolBEstrella.hpp
 * @brief Declaración de la plantilla de clase ArbolBEstrella<>.
 *
 * Implementa una estructura de árbol B* genérica mediante nodos con arreglos
 * dinámicos de claves e hijos. La clase administra la raíz, las operaciones
 * públicas de búsqueda, inserción, eliminación, vaciado e impresión, y los
 * auxiliares privados usados para redistribuir, dividir y fusionar nodos.
 *
 * @date 18/06/2026
 */

#ifndef B_ESTRELLA_ARBOLBESTRELLA_HPP
#define B_ESTRELLA_ARBOLBESTRELLA_HPP

#include "Nodo.hpp"
#include <exception>

/**
 * \brief Representa un árbol B* genérico.
 *
 * Un árbol B* mantiene sus claves ordenadas dentro de cada nodo y utiliza
 * operaciones de redistribución, división y fusión para conservar sus
 * restricciones de ocupación después de insertar o eliminar elementos.
 *
 * \tparam T Tipo de dato almacenado como clave. Debe soportar comparación
 *         mediante los operadores <, > y ==.
 */
template <typename T>
class ArbolBEstrella {
public:
    /**
     * \brief Excepción que se lanza cuando falla una reserva de memoria.
     *
     * Hereda de std::exception para permitir captura polimórfica con
     * catch(const std::exception &).
     */
    class MemoriaInsuficiente : public std::exception {
    public:
        /** \throw No lanza ninguna excepción. */
        MemoriaInsuficiente() throw();

        /**
         * \brief Devuelve el mensaje descriptivo de la excepción.
         * \return const char* Mensaje fijo de error.
         * \throw No lanza ninguna excepción.
         */
        virtual const char* what() const throw();
    };

    /**
     * \brief Constructor con orden específico.
     *
     * Inicializa un árbol vacío. El orden determina la cantidad máxima de hijos
     * por nodo y, por consecuencia, el máximo de claves es orden - 1.
     *
     * \param orden unsigned int Orden del árbol.
     */
    ArbolBEstrella(unsigned int orden);

    /**
     * \brief Constructor de copia.
     *
     * Crea una copia profunda del árbol recibido, duplicando sus nodos y
     * reconstruyendo los apuntadores a padre.
     *
     * \param arbol const ArbolBEstrella& Árbol origen.
     * \throw MemoriaInsuficiente Si falla la copia de algún nodo.
     */
    ArbolBEstrella(const ArbolBEstrella &arbol);

    /**
     * \brief Operador de asignación.
     *
     * Libera el contenido actual y lo reemplaza por una copia profunda de
     * \a arbol. Protege contra autoasignación.
     *
     * \param arbol const ArbolBEstrella& Árbol origen.
     * \return ArbolBEstrella& Referencia al árbol actual.
     * \throw MemoriaInsuficiente Si falla la copia de algún nodo.
     */
    ArbolBEstrella& operator=(const ArbolBEstrella &arbol);

    /**
     * \brief Destructor.
     *
     * Libera todos los nodos del árbol mediante un recorrido recursivo desde la
     * raíz.
     */
    ~ArbolBEstrella();

    /**
     * \brief Busca una clave en el árbol.
     * \param clave T Clave a buscar.
     * \return bool true si la clave existe, false en caso contrario.
     */
    bool buscar(T clave) const;

    /**
     * \brief Inserta una clave en el árbol.
     *
     * Si la clave ya existe, no modifica el árbol. Si un nodo se llena,
     * intenta aplicar redistribución B* o división según corresponda.
     *
     * \param clave T Clave a insertar.
     * \return bool true si la clave se insertó, false si ya existía.
     * \throw MemoriaInsuficiente Si falla alguna reserva de memoria.
     */
    bool insertar(T clave);

    /**
     * \brief Elimina una clave del árbol.
     *
     * Después de eliminar, corrige deficiencias por redistribución o fusión y
     * ajusta la raíz si queda vacía.
     *
     * \param clave T Clave a eliminar.
     * \return bool true si la clave se eliminó, false si no existía.
     */
    bool eliminar(T clave);

    /**
     * \brief Vacía el árbol completo.
     *
     * Destruye todos los nodos y deja la raíz en nullptr.
     */
    void vaciar();

    /**
     * \brief Imprime el árbol en consola.
     *
     * Muestra primero el subárbol derecho, luego la clave actual y después el
     * subárbol izquierdo, generando una vista rotada por niveles.
     */
    void imprimir() const;

    /**
     * \brief Imprime las claves del árbol de izquierda a derecha.
     *
     * Realiza un recorrido in-order generalizado para árboles B/B*.
     * Esto significa que visita primero el hijo izquierdo de cada clave,
     * después imprime la clave, y finalmente continúa con el siguiente hijo.
     *
     * La salida muestra las claves en orden ascendente, separadas por espacios.
     * Si el árbol está vacío, imprime el mensaje "Árbol vacío".
     */
    void imprimirIzquierdaDerecha() const;

private:
    int orden_;        ///< Orden del árbol; corresponde al máximo de hijos por nodo.
    int max_claves_;   ///< Número máximo de claves permitidas por nodo.
    Nodo<T> *raiz_;    ///< Apuntador a la raíz del árbol; nullptr si está vacío.

    // =====================================================================
    // Manejo de memoria y copia
    // =====================================================================

    /**
     * \brief Destruye recursivamente un subárbol.
     * \param nodo Nodo<T>* Raíz del subárbol a destruir.
     */
    void destruir(Nodo<T>* nodo);

    /**
     * \brief Crea una copia profunda de un subárbol.
     * \param nodo Nodo<T>* Raíz del subárbol origen.
     * \return Nodo<T>* Raíz del nuevo subárbol copiado.
     * \throw MemoriaInsuficiente Si falla la reserva de memoria.
     */
    Nodo<T>* copiarNodo(Nodo<T> *nodo);

    // =====================================================================
    // Búsqueda
    // =====================================================================

    /**
     * \brief Busca una clave dentro de un subárbol.
     *
     * Además de indicar si la clave existe, deja en \a auxiliar el último nodo
     * visitado y en \a indice la posición donde se encontró o donde debería ir.
     *
     * \param subraiz Nodo<T>* Raíz del subárbol donde se busca.
     * \param clave T Clave a buscar.
     * \param auxiliar Nodo<T>*& Último nodo visitado durante la búsqueda.
     * \param indice int& Índice encontrado o posición tentativa de inserción.
     * \return bool true si la clave existe, false en caso contrario.
     */
    bool buscar(Nodo<T> *subraiz, T clave, Nodo<T>* &auxiliar, int &indice) const;

    // =====================================================================
    // Auxiliares generales de arreglos y nodos
    // =====================================================================

    /**
     * \brief Inserta un elemento en un arreglo ordenado de claves.
     * \param arreglo T[] Arreglo donde se insertará la clave.
     * \param num_elem int Número actual de elementos válidos.
     * \param elem_insertar const T& Elemento a insertar.
     * \return int Índice donde quedó insertado el elemento.
     */
    int insertarEnOrden(T arreglo[], int num_elem, const T &elem_insertar);

    /**
     * \brief Inserta un apuntador a nodo en un índice específico de un arreglo.
     * \param arreglo_nodos Nodo<T>** Arreglo de apuntadores a nodos.
     * \param num_nodos int Número actual de nodos válidos.
     * \param nodo_insertar Nodo<T>* Nodo que se insertará.
     * \param indice_insercion int Índice donde se insertará el nodo.
     */
    void insertarEnIndice(Nodo<T> **arreglo_nodos, int num_nodos, Nodo<T> *nodo_insertar, int indice_insercion);

    /**
     * \brief Obtiene el índice de un hijo dentro de un arreglo de hijos.
     * \param arreglo_hijos Nodo<T>** Arreglo donde se buscará el nodo.
     * \param num_hijos int Número de hijos válidos a revisar.
     * \param nodo_buscado Nodo<T>* Nodo cuyo índice se desea encontrar.
     * \return int Índice del nodo buscado, o -1 si no se encuentra.
     */
    int obtenerIndice(Nodo<T> **arreglo_hijos, int num_hijos, Nodo<T> *nodo_buscado) const;

    // =====================================================================
    // Inserción y balanceo B*
    // =====================================================================

    /**
     * \brief Redistribuye claves entre dos hijos hermanos y su padre.
     * \param nodo_padre Nodo<T>* Padre de los dos hijos.
     * \param nodo_hijo_izq Nodo<T>* Hijo izquierdo participante.
     * \param nodo_hijo_der Nodo<T>* Hijo derecho participante.
     * \param indice_nodo_hijo_izq int Índice del hijo izquierdo en el padre.
     */
    void redistribuir(Nodo<T> *nodo_padre, Nodo<T> *nodo_hijo_izq, Nodo<T> *nodo_hijo_der, int indice_nodo_hijo_izq);

    /**
     * \brief Divide dos nodos hermanos llenos en tres nodos.
     *
     * Corresponde al caso B* 2->3 durante inserción.
     *
     * \param nodo_padre Nodo<T>* Padre de los hijos que se dividen.
     * \param nodo_hijo_izq Nodo<T>* Hijo izquierdo participante.
     * \param nodo_hijo_der Nodo<T>* Hijo derecho participante.
     * \param indice_nodo_hijo_izq int Índice del hijo izquierdo en el padre.
     * \throw MemoriaInsuficiente Si falla la reserva del nuevo nodo.
     */
    void dividir(Nodo<T> *nodo_padre, Nodo<T> *nodo_hijo_izq, Nodo<T> *nodo_hijo_der, int indice_nodo_hijo_izq);

    /**
     * \brief Fusiona tres hijos en dos nodos.
     *
     * Corresponde al caso B* 3->2 usado por esta implementación para
     * redistribuir contenido cuando es posible reducir un hijo.
     *
     * \param padre Nodo<T>* Padre de los tres hijos.
     * \param indice int Índice del primer hijo participante.
     */
    void fusionar(Nodo<T> *padre, int indice);

    /**
     * \brief Divide la raíz cuando esta se encuentra llena.
     * \param vieja_raiz Nodo<T>* Raíz anterior que será dividida.
     * \throw MemoriaInsuficiente Si falla la creación de los nuevos nodos.
     */
    void dividirRaiz(Nodo<T> *vieja_raiz);

    // =====================================================================
    // Eliminación y corrección de deficiencias
    // =====================================================================

    /**
     * \brief Elimina una clave dentro de un subárbol.
     * \param subraiz Nodo<T>* Raíz del subárbol donde se elimina.
     * \param clave T Clave a eliminar.
     * \return bool true si se eliminó, false si no se encontró.
     */
    bool eliminar(Nodo<T> *subraiz, T clave);

    /**
     * \brief Obtiene el sucesor inmediato de una clave interna.
     * \param nodo Nodo<T>* Nodo que contiene la clave.
     * \param indice int Índice de la clave dentro del nodo.
     * \return T Sucesor de la clave.
     */
    T obtenerSucesor(Nodo<T> *nodo, int indice) const;

    /**
     * \brief Obtiene el predecesor inmediato de una clave interna.
     * \param nodo Nodo<T>* Nodo que contiene la clave.
     * \param indice int Índice de la clave dentro del nodo.
     * \return T Predecesor de la clave.
     */
    T obtenerPredecesor(Nodo<T> *nodo, int indice) const;

    /**
     * \brief Indica si un nodo quedó por debajo del mínimo de claves.
     * \param nodo Nodo<T>* Nodo a revisar.
     * \return bool true si el nodo es deficiente, false en caso contrario.
     */
    bool esDeficiente(Nodo<T> *nodo) const;

    /**
     * \brief Corrige la deficiencia de un hijo después de eliminar.
     *
     * Intenta redistribuir con hermanos y, si no es suficiente, fusiona nodos.
     *
     * \param nodo_padre Nodo<T>* Padre del hijo deficiente.
     * \param indice_hijo int Índice del hijo deficiente dentro del padre.
     */
    void corregirDeficiencia(Nodo<T> *nodo_padre, int indice_hijo);

    /**
     * \brief Calcula el mínimo de claves usado por esta implementación.
     * \return int Cantidad mínima de claves para nodos no raíz.
     */
    int minimoClaves() const;

    /**
     * \brief Ajusta la raíz si queda vacía después de una eliminación.
     */
    void ajustarRaizDespuesDeEliminar();

    /**
     * \brief Fusiona dos hijos usando la clave separadora del padre.
     * \param nodo_padre Nodo<T>* Padre de los hijos.
     * \param indice_hijo_izq int Índice del hijo izquierdo.
     */
    void fusionarDosHijos(Nodo<T> *nodo_padre, int indice_hijo_izq);

    // =====================================================================
    // Impresión
    // =====================================================================

    /**
     * \brief Imprime recursivamente un subárbol con indentación por nivel.
     * \param nodo Nodo<T>* Nodo raíz del subárbol a imprimir.
     * \param nivel int Nivel de profundidad usado para indentar.
     */
    void imprimir(Nodo<T> *nodo, int nivel) const;

    /**
     * \brief Función auxiliar recursiva para imprimir el árbol de izquierda a derecha.
     *
     * Recorre el subárbol recibido siguiendo el orden:
     *
     * hijo[0], clave[0], hijo[1], clave[1], ..., clave[k-1], hijo[k]
     *
     * donde k es el número de claves del nodo actual.
     *
     * \param nodo Nodo<T>* Nodo actual desde el cual se continúa el recorrido.
     * \param primero bool& Bandera que indica si todavía no se ha impreso ninguna clave.
     *
     * \note El parámetro @a primero se pasa por referencia para conservar su valor
     * durante todas las llamadas recursivas y evitar imprimir separadores extra.
     */
    void imprimirIzquierdaDerecha(Nodo<T> *nodo, bool &primero) const;

};

#include "ArbolBEstrella.tpp"

#endif //B_ESTRELLA_ARBOLBESTRELLA_HPP
