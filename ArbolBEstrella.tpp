/**
 * @file ArbolBEstrella.tpp
 * @brief Implementación de la plantilla de clase ArbolBEstrella<>.
 *
 * Contiene las definiciones de las operaciones públicas del árbol B* y de los
 * auxiliares privados para búsqueda, inserción, eliminación, balanceo, copia y
 * liberación de memoria.
 *
 * @date 18/06/2026
 */

#include <iostream>
#include <new>

// ============================================================================
//  Implementación de clases de excepción
// ============================================================================

/** \brief Constructor de la excepción MemoriaInsuficiente. */
template <typename T>
ArbolBEstrella<T>::MemoriaInsuficiente::MemoriaInsuficiente() throw(){}

// ------------------------------------------------------- //
/** \brief Devuelve el mensaje asociado a MemoriaInsuficiente. */
template <typename T>
const char* ArbolBEstrella<T>::MemoriaInsuficiente::what() const throw()
{
    return "Memoria insuficiente";
}

// ============================================================================
//  Regla de los tres
// ============================================================================

/** \brief Inicializa un árbol B* vacío con el orden indicado. */
template<typename T>
ArbolBEstrella<T>::ArbolBEstrella(unsigned int orden) : orden_(orden), max_claves_(orden - 1), raiz_(nullptr) {}

// ------------------------------------------------------- //
/** \brief Construye una copia profunda de otro árbol B*. */
template<typename T>
ArbolBEstrella<T>::ArbolBEstrella(const ArbolBEstrella &arbol) : orden_(arbol.orden_), max_claves_(arbol.orden_ - 1), raiz_(nullptr) {
    *this = arbol;
}

// ------------------------------------------------------- //
/** \brief Asigna al árbol actual una copia profunda de otro árbol B*. */
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

// ------------------------------------------------------- //
/** \brief Libera todos los nodos pertenecientes al árbol. */
template<typename T>
ArbolBEstrella<T>::~ArbolBEstrella() {
    destruir(raiz_);
}

// ============================================================================
//  Métodos públicos
// ============================================================================

/** \brief Busca una clave desde la raíz del árbol. */
template<typename T>
bool ArbolBEstrella<T>::buscar(T clave) const {
    Nodo<T> *auxiliar = nullptr;
    int i = 0;
    return buscar(raiz_, clave, auxiliar, i);
}

// ------------------------------------------------------- //
/** \brief Inserta una clave y aplica el balanceo necesario. */
template <typename T>
bool ArbolBEstrella<T>::insertar(T clave)
{
    try
    {
        //Si el árbol está vacío, crear un nodo para la raíz
        if(raiz_ == nullptr) {
            // La raíz recién creada siempre es una hoja al principio
            raiz_ = new Nodo<T>(orden_, true);
            insertarEnOrden(raiz_->claves_, raiz_->num_claves_, clave);
            raiz_->num_claves_ = 1;
            return true;
        }

        //Buscar la clave en el árbol y guardar en una variable la dirección del nodo y del índice donde está o debería estar la clave
        Nodo<T> *nodo_clave = nullptr;
        int indice_clave = 0;
        bool esta_la_clave = buscar(raiz_, clave, nodo_clave, indice_clave);

        //Si la clave ya está en el árbol, devolver falso
        if(esta_la_clave) {
            return false;
        }

        //Si no lo está, insertar la clave en el nodo hoja correspondiente
        insertarEnOrden(nodo_clave->claves_, nodo_clave->num_claves_, clave);
        nodo_clave->num_claves_++;

        Nodo<T> *nodo_actual = nodo_clave;

        //Repetir indefinidamente
        while(true){
            //Si el nodo se sobrecargó a causa de la inserción de la clave
            if(nodo_actual->num_claves_ > max_claves_){
                //Si el nodo es la raíz
                if(nodo_actual->padre_ == nullptr){
                    //Dividir la raíz y terminar el ciclo
                    dividirRaiz(raiz_);
                    break;
                }
                else{
                    /*Si algún hermano del nodo tiene espacio, redistribuir las claves y terminar el ciclo
                    Para hacer esto primero debemos obtener el indice que el nodo actual ocupa del arreglo de hijos del padre*/

                    //Obtener el indice del nodo actual
                    int indice_nodo_actual = obtenerIndice(nodo_actual->padre_->hijos_, nodo_actual->padre_->num_claves_ + 1, nodo_actual);

                    //Si el nodo actual tiene un hermano izquerdo y este tiene espacio, redistribuir
                    if(indice_nodo_actual > 0){
                        Nodo<T> *hno_izq_nodo_actual = nodo_actual->padre_->hijos_[indice_nodo_actual - 1];

                        //Si el hermano izquierdo tiene espacio, redistrinuir
                        if(hno_izq_nodo_actual->num_claves_ < max_claves_){
                            redistribuir(nodo_actual->padre_, hno_izq_nodo_actual, nodo_actual, indice_nodo_actual - 1);
                            break;
                        }
                    }

                    /*Si llegamos a esta linea, significa que el nodo actual no tiene hermano izquierdo o este estaba lleno, asi que probamos
                    con el hermano derecho*/
                    if(indice_nodo_actual < nodo_actual->padre_->num_claves_){
                        Nodo<T> *hno_der_nodo_actual = nodo_actual->padre_->hijos_[indice_nodo_actual + 1];

                        //Si el hermano derecho tiene espacio, redistribuir
                        if(hno_der_nodo_actual->num_claves_ < max_claves_){
                            redistribuir(nodo_actual->padre_, nodo_actual, hno_der_nodo_actual, indice_nodo_actual);
                            break;
                        }
                    }

                    //Si llegamos a este punto, no se pudo hacer ninguna redistribución. Toca dividir dos nodos a tres.
                    if(indice_nodo_actual > 0){
                        dividir(nodo_actual->padre_, nodo_actual->padre_->hijos_[indice_nodo_actual - 1], nodo_actual, indice_nodo_actual - 1);
                    }else{
                        dividir(nodo_actual->padre_, nodo_actual, nodo_actual->padre_->hijos_[indice_nodo_actual + 1], indice_nodo_actual);
                    }

                    //Hacer que ahora el nodo sea el padre, para ver si se sobrecargó y continuar la siguiente ejecución del ciclo
                    nodo_actual = nodo_actual->padre_;
                    continue;
                }
            }
            // Si no se sobrecargó, entonces terminar el ciclo
            else{
                break;
            }
        }
        return true;
    }
    catch(std::bad_alloc &error)
    {
        throw MemoriaInsuficiente();
    }
}

// ------------------------------------------------------- //
/** \brief Elimina una clave desde la raíz del árbol. */
template <typename T>
bool ArbolBEstrella<T>::eliminar(T clave) {
    if (raiz_ == nullptr) return false;

    bool resultado = eliminar(raiz_, clave);

    ajustarRaizDespuesDeEliminar();

    return resultado;
}

// ------------------------------------------------------- //
/** \brief Destruye todos los nodos y deja el árbol vacío. */
template <typename T>
void ArbolBEstrella<T>::vaciar() {
    // Si el árbol ya está vacío, no hacemos nada.
    if (raiz_ == nullptr) {
        return;
    }

    // Destruye recursivamente todo el árbol desde la raíz.
    destruir(raiz_);

    // Después de destruir, raiz_ debe quedar en nullptr
    // para evitar que apunte a memoria ya liberada.
    raiz_ = nullptr;
}

// ------------------------------------------------------- //
/** \brief Imprime el árbol completo en consola. */
template <typename T>
void ArbolBEstrella<T>::imprimir() const { imprimir(raiz_, 0); }

// ============================================================================
//  Manejo de memoria y copia
// ============================================================================

/** \brief Destruye recursivamente un subárbol. */
template<typename T>
void ArbolBEstrella<T>::destruir(Nodo<T>* nodo) {
    if (nodo == nullptr) return;
    for (int i = 0; i < nodo->num_claves_+ 1; ++i) {
        destruir(nodo->hijos_[i]); //eliminamos los hijos antes que los padres
    }
    delete nodo;
}

// ------------------------------------------------------- //
/** \brief Copia recursivamente un subárbol y reconstruye padres. */
template<typename T>
Nodo<T>* ArbolBEstrella<T>::copiarNodo(Nodo<T> *nodo) {
    if (nodo == nullptr) return nullptr;

    // Reservamos el nodo, si falla, lo reportamos como MemoriaInsuficiente
    // (consistente con el resto del arbol, en vez de devolver nullptr en silencio.
    Nodo<T> *nuevo = nullptr;
    try {
        nuevo = new Nodo<T>(this->orden_, nodo->es_hoja_);
    }
    catch (const std::bad_alloc&) {
        throw MemoriaInsuficiente();
    }

    // Copiamos claves e hijos. Si una copia recursiva falla a media construccion,
    // liberamos el subarbol parcial para no dejar memoria colgada y propagamos.
    try {
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
    }
    catch (...) {
        destruir(nuevo);   // libera el subarbol parcial ya construido
        throw;             // propaga MemoriaInsuficiente hacia arriba
    }

    return nuevo;
}

// ============================================================================
//  Búsqueda
// ============================================================================

/** \brief Busca una clave en un subárbol y devuelve el nodo e índice visitados. */
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

// ============================================================================
//  Auxiliares generales de arreglos y nodos
// ============================================================================

/** \brief Inserta una clave en orden dentro de un arreglo. */
template <typename T>
int ArbolBEstrella<T>::insertarEnOrden(T arreglo[], int num_elem, const T &elem_insertar)
{
    //Hacer que el indice actual sea el indice del ultimo elemento del arreglo
    int indice_actual = num_elem - 1;

    //Mover una posicion a la derecha todos los elementos mayores que el elemento a insertar
    while(indice_actual >= 0 && arreglo[indice_actual] > elem_insertar){
        arreglo[indice_actual + 1] = arreglo[indice_actual];
        indice_actual = indice_actual - 1;
    }

    arreglo[indice_actual + 1] = elem_insertar;

    return indice_actual + 1;
}

// ------------------------------------------------------- //
/** \brief Inserta un apuntador a nodo en un arreglo de hijos. */
template <typename T>
void ArbolBEstrella<T>::insertarEnIndice(Nodo<T> **arreglo_nodos, int num_nodos, Nodo<T> *nodo_insertar, int indice_insercion)
{
    //Mover una posicion a la derecha todos los elementos que estan a partir el indice a la derecha
    for(int i = num_nodos - 1; i >= indice_insercion; i--){
        arreglo_nodos[i + 1] = arreglo_nodos[i];
    }

    //Una vez hecho el hueco, insertar el nuevo elemento
    arreglo_nodos[indice_insercion] = nodo_insertar;
}

// ------------------------------------------------------- //
/** \brief Localiza un nodo dentro de un arreglo de hijos. */
template <typename T>
int ArbolBEstrella<T>::obtenerIndice(Nodo<T> **arreglo_hijos, int num_hijos, Nodo<T> *nodo_buscado) const
{
    //Recorrer los hijos
    for(int i = 0; i < num_hijos; ++i){
        //Si la direccion del hijo del arreglo es la misma que la del nodo buscado, devolvemos el indice
        if(arreglo_hijos[i] == nodo_buscado){
            return i;
        }
    }

    //Si no encuentra al hijo buscado, devolver -1
    return -1;
}

// ============================================================================
//  Inserción y balanceo B*
// ============================================================================

/** \brief Redistribuye claves e hijos entre dos hermanos. */
template <typename T>
void ArbolBEstrella<T>::redistribuir(
    Nodo<T> *nodo_padre,
    Nodo<T> *nodo_hijo_izq,
    Nodo<T> *nodo_hijo_der,
    int indice_nodo_hijo_izq
)
{
    try
    {
        // Guardamos los tamaños originales.
        int num_claves_nodo_hijo_izq = nodo_hijo_izq->num_claves_;
        int num_claves_nodo_hijo_der = nodo_hijo_der->num_claves_;

        // Vamos a juntar:
        // claves del hijo izquierdo
        // + clave separadora del padre
        // + claves del hijo derecho.
        int tam_arreglo_aux = num_claves_nodo_hijo_izq + num_claves_nodo_hijo_der + 1;

        // Arreglo temporal para las claves.
        T *arreglo_aux = new T[tam_arreglo_aux];

        // Arreglo temporal para hijos internos.
        // Solo se usa si los nodos no son hojas.
        Nodo<T> **arreglo_hijos_aux = nullptr;
        int tam_arreglo_aux_hijos = tam_arreglo_aux + 1;

        if (nodo_hijo_izq->es_hoja_ == false) {
            arreglo_hijos_aux = new Nodo<T>*[tam_arreglo_aux_hijos];
        }

        int pos = 0;

        // Copiamos claves del hijo izquierdo.
        for (int i = 0; i < num_claves_nodo_hijo_izq; ++i) {
            arreglo_aux[pos++] = nodo_hijo_izq->claves_[i];
        }

        // Copiamos la clave separadora del padre.
        arreglo_aux[pos++] = nodo_padre->claves_[indice_nodo_hijo_izq];

        // Copiamos claves del hijo derecho.
        for (int i = 0; i < num_claves_nodo_hijo_der; ++i) {
            arreglo_aux[pos++] = nodo_hijo_der->claves_[i];
        }

        // Si no son hojas, también juntamos todos los hijos.
        if (nodo_hijo_izq->es_hoja_ == false) {
            int pos_h = 0;

            // Hijos del nodo izquierdo.
            for (int i = 0; i <= num_claves_nodo_hijo_izq; ++i) {
                arreglo_hijos_aux[pos_h++] = nodo_hijo_izq->hijos_[i];
            }

            // Hijos del nodo derecho.
            for (int i = 0; i <= num_claves_nodo_hijo_der; ++i) {
                arreglo_hijos_aux[pos_h++] = nodo_hijo_der->hijos_[i];
            }
        }

        // Una clave del arreglo combinado se queda como separadora en el padre.
        int num_claves_a_repartir = tam_arreglo_aux - 1;

        // El nodo izquierdo recibe la mitad inferior.
        int num_claves_nuevo_izq = num_claves_a_repartir / 2;

        // Esta posición será la nueva clave separadora del padre.
        int indice_separadora = num_claves_nuevo_izq;

        // El nodo derecho recibe el resto.
        int num_claves_nuevo_der = num_claves_a_repartir - num_claves_nuevo_izq;

        // Reescribimos claves del nodo izquierdo.
        for (int i = 0; i < num_claves_nuevo_izq; ++i) {
            nodo_hijo_izq->claves_[i] = arreglo_aux[i];
        }

        // Actualizamos cantidad de claves del izquierdo.
        nodo_hijo_izq->num_claves_ = num_claves_nuevo_izq;

        // Actualizamos la clave separadora del padre.
        nodo_padre->claves_[indice_nodo_hijo_izq] = arreglo_aux[indice_separadora];

        // Reescribimos claves del nodo derecho.
        for (int i = 0; i < num_claves_nuevo_der; ++i) {
            nodo_hijo_der->claves_[i] = arreglo_aux[indice_separadora + 1 + i];
        }

        // Actualizamos cantidad de claves del derecho.
        nodo_hijo_der->num_claves_ = num_claves_nuevo_der;

        // Si los nodos son internos, redistribuimos también sus hijos.
        if (nodo_hijo_izq->es_hoja_ == false) {
            // El izquierdo necesita num_claves_nuevo_izq + 1 hijos.
            for (int i = 0; i <= num_claves_nuevo_izq; ++i) {
                nodo_hijo_izq->hijos_[i] = arreglo_hijos_aux[i];

                // Actualizamos el padre de cada hijo.
                if (nodo_hijo_izq->hijos_[i] != nullptr) {
                    nodo_hijo_izq->hijos_[i]->padre_ = nodo_hijo_izq;
                }
            }

            // El derecho necesita num_claves_nuevo_der + 1 hijos.
            for (int i = 0; i <= num_claves_nuevo_der; ++i) {
                nodo_hijo_der->hijos_[i] = arreglo_hijos_aux[indice_separadora + 1 + i];

                // Actualizamos el padre de cada hijo.
                if (nodo_hijo_der->hijos_[i] != nullptr) {
                    nodo_hijo_der->hijos_[i]->padre_ = nodo_hijo_der;
                }
            }

            // Limpiamos punteros sobrantes del izquierdo.
            for (int i = num_claves_nuevo_izq + 1; i <= orden_; ++i) {
                nodo_hijo_izq->hijos_[i] = nullptr;
            }

            // Limpiamos punteros sobrantes del derecho.
            for (int i = num_claves_nuevo_der + 1; i <= orden_; ++i) {
                nodo_hijo_der->hijos_[i] = nullptr;
            }
        }

        // Liberamos memoria temporal.
        delete[] arreglo_aux;
        delete[] arreglo_hijos_aux;
    }
    catch (std::bad_alloc &error)
    {
        throw MemoriaInsuficiente();
    }
}

// ------------------------------------------------------- //
/** \brief Divide dos hermanos en tres nodos durante inserción B*. */
template <typename T>
void ArbolBEstrella<T>::dividir(Nodo<T> *nodo_padre, Nodo<T> *nodo_hijo_izq, Nodo<T> *nodo_hijo_der, int indice_nodo_hijo_izq)
{
    try
    {
        //Crear un nuevo nodo
        Nodo<T> *nuevo_nodo = new Nodo<T>(orden_, nodo_hijo_izq->es_hoja_);

        //Crear arreglo auxiliar para claves
        int num_claves_nodo_hijo_izq = nodo_hijo_izq->num_claves_;
        int num_claves_nodo_hijo_der = nodo_hijo_der->num_claves_;
        int tam_arreglo_aux = num_claves_nodo_hijo_izq + num_claves_nodo_hijo_der + 1;
        T *arreglo_aux = new T[tam_arreglo_aux];

        //Crear un arreglo auxiliar para los hijos
        int num_hijos_nodo_hijo_izq = num_claves_nodo_hijo_izq + 1;
        int num_hijos_nodo_hijo_der = num_claves_nodo_hijo_der + 1;
        int tam_arreglo_aux_hijos = num_hijos_nodo_hijo_izq + num_hijos_nodo_hijo_der;
        Nodo<T> **arreglo_hijos_aux = nullptr;

        if(nodo_hijo_izq->es_hoja_ == false){
            arreglo_hijos_aux = new Nodo<T>*[tam_arreglo_aux_hijos];
        }


        //Poner claves del hijo izquierdo
        for(int i = 0; i < num_claves_nodo_hijo_izq; ++i){
            arreglo_aux[i] = nodo_hijo_izq->claves_[i];
        }

        //Poner clave del padre
        arreglo_aux[num_claves_nodo_hijo_izq] = nodo_padre->claves_[indice_nodo_hijo_izq];

        //Poner claves del hijo derecho
        int ultimo_indice_arreglo_aux = num_claves_nodo_hijo_izq;
        for(int i = 0; i < num_claves_nodo_hijo_der; ++i){
            arreglo_aux[ultimo_indice_arreglo_aux + 1 + i] = nodo_hijo_der->claves_[i];
        }

        //En este punto, hemos agregado al arreglo auxiliar, todas las claves de los nodos hijos y la del padre

        //Si los nodos tienen hijos, entonces agregar al arreglo auxiliar de hijos los hijos de los nodos hijos
        if(nodo_hijo_izq->es_hoja_ == false){

            // Agregar los hijos del nodo izquierdo
            for(int i = 0; i < num_hijos_nodo_hijo_izq; ++i){
                arreglo_hijos_aux[i] = nodo_hijo_izq->hijos_[i];
            }

            // Agregar los hijos del nodo derecho
            for(int i = 0; i < num_hijos_nodo_hijo_der; ++i){
                arreglo_hijos_aux[num_hijos_nodo_hijo_izq + i] = nodo_hijo_der->hijos_[i];
            }
        }

        //Calcular cuantas claves tendrá cada uno de los tres nodos hijos
        int num_claves_a_repartir = tam_arreglo_aux - 2;
        int num_claves_nodo_1 = num_claves_a_repartir / 3;
        int num_claves_nodo_2 = (num_claves_a_repartir - num_claves_nodo_1) / 2;
        int num_claves_nodo_3 = num_claves_a_repartir - num_claves_nodo_1 - num_claves_nodo_2;

        //Calcular los índices de las claves que estarán en el padre funcionando como separadoras
        int indice_1 = num_claves_nodo_1;
        int indice_2 = num_claves_nodo_1 + num_claves_nodo_2 + 1;

        //REPARTIR LAS CLAVES A LOS 3 NODOS HIJOS

        //Mudar las claves que van en el nodo 1
        for(int i = 0; i < num_claves_nodo_1; ++i){
            nodo_hijo_izq->claves_[i] = arreglo_aux[i];
        }

        //Si los nodos tienen hijos, mudar los hijos que van en el nodo 1
        if(nodo_hijo_izq->es_hoja_ == false){
            for(int i = 0; i < (num_claves_nodo_1 + 1); ++i){
                nodo_hijo_izq->hijos_[i] = arreglo_hijos_aux[i];
                nodo_hijo_izq->hijos_[i]->padre_ = nodo_hijo_izq;
            }
        }

        //Mudar claves que van en el nodo 2
        for(int i = 0; i < num_claves_nodo_2; ++i){
            nuevo_nodo->claves_[i] = arreglo_aux[indice_1 + 1 + i];
        }

        //Si los nodos tienen hijos, mudar los hijos que van en el nodo 2
        if(nodo_hijo_izq->es_hoja_ == false){
            for(int i = 0; i < (num_claves_nodo_2 + 1); ++i){
                nuevo_nodo->hijos_[i] = arreglo_hijos_aux[num_claves_nodo_1 + 1 + i];
                nuevo_nodo->hijos_[i]->padre_ = nuevo_nodo;
            }
        }

        //Mudar las claves que van en el nodo 3
        for(int i = 0; i < num_claves_nodo_3; ++i){
            nodo_hijo_der->claves_[i] = arreglo_aux[indice_2 + 1 + i];
        }

        //Si los nodos tiene hijos, mudar los hijos que van en el nodo 3
        if(nodo_hijo_izq->es_hoja_ == false){
            for(int i = 0; i < (num_claves_nodo_3 + 1); ++i){
                nodo_hijo_der->hijos_[i] = arreglo_hijos_aux[num_claves_nodo_1 + num_claves_nodo_2 + 2 + i];
                nodo_hijo_der->hijos_[i]->padre_ = nodo_hijo_der;
            }
        }

        //Actualizar contadores de claves
        nodo_hijo_izq->num_claves_ = num_claves_nodo_1;
        nuevo_nodo->num_claves_ = num_claves_nodo_2;
        nodo_hijo_der->num_claves_ = num_claves_nodo_3;

        //Para este punto ya tenemos los tres nodos hijos con sus respectivas claves y sus respectivos hijos

        //ACTUALIZACION DEL PADRE

        //Reemplazar en el padre la primera clave
        nodo_padre->claves_[indice_nodo_hijo_izq] = arreglo_aux[indice_1];

        //Insertar la segunda clave
         int indice_clave_2 = insertarEnOrden(nodo_padre->claves_, nodo_padre->num_claves_, arreglo_aux[indice_2]);

         //Actualizar el numero de claves del padre
        nodo_padre->num_claves_ = nodo_padre->num_claves_ + 1;

        //Insertar el nuevo nodo en el arreglo de hijos del padre
        insertarEnIndice(nodo_padre->hijos_, nodo_padre->num_claves_, nuevo_nodo, indice_clave_2);
        nuevo_nodo->padre_ = nodo_padre;

        delete[] arreglo_aux;
        if(arreglo_hijos_aux != nullptr){
            delete[] arreglo_hijos_aux;
        }
    }
    catch(std::bad_alloc &error)
    {
        throw MemoriaInsuficiente();
    }
}

// ------------------------------------------------------- //
/** \brief Fusiona tres hijos en dos nodos conservando una separadora en el padre. */
template <typename T>
void ArbolBEstrella<T>::fusionar(Nodo<T> *padre, int indice) {
    // Validación básica: no podemos fusionar si no hay padre.
    if (padre == nullptr) {
        return;
    }

    // Para fusionar necesitamos tres hijos:
    // padre->hijos_[indice]
    // padre->hijos_[indice + 1]
    // padre->hijos_[indice + 2]
    //
    // También necesitamos dos claves separadoras:
    // padre->claves_[indice]
    // padre->claves_[indice + 1]
    //
    // Por eso indice + 1 debe existir dentro de las claves del padre.
    if (indice < 0 || indice + 1 >= padre->num_claves_) {
        return;
    }

    // Tomamos los tres hijos que van a participar en la fusión B*.
    Nodo<T> *izq = padre->hijos_[indice];
    Nodo<T> *medio = padre->hijos_[indice + 1];
    Nodo<T> *der = padre->hijos_[indice + 2];

    // Si alguno no existe, no podemos hacer la operación.
    if (izq == nullptr || medio == nullptr || der == nullptr) {
        return;
    }

    // Guardamos si son hojas.
    // Si son hojas, no necesitamos mover punteros a hijos.
    const bool son_hojas = izq->es_hoja_;

    // Guardamos los tamaños originales antes de modificar los nodos.
    int n_izq = izq->num_claves_;
    int n_medio = medio->num_claves_;
    int n_der = der->num_claves_;

    // Total de claves que vamos a juntar:
    // claves del hijo izquierdo
    // + clave separadora 1 del padre
    // + claves del hijo medio
    // + clave separadora 2 del padre
    // + claves del hijo derecho.
    int total_claves = n_izq + n_medio + n_der + 2;

    // Después de la fusión B*, una clave se queda como separadora en el padre.
    // Entonces los dos hijos resultantes reciben total_claves - 1 claves.
    //
    // Si eso no cabe en dos nodos, no hacemos la operación.
    if (total_claves - 1 > 2 * max_claves_) {
        return;
    }

    // Arreglo temporal para juntar todas las claves en orden.
    T *claves_aux = new T[total_claves];

    int pos = 0;

    // Copiamos claves del hijo izquierdo.
    for (int i = 0; i < n_izq; ++i) {
        claves_aux[pos++] = izq->claves_[i];
    }

    // Copiamos la primera clave separadora del padre.
    claves_aux[pos++] = padre->claves_[indice];

    // Copiamos claves del hijo medio.
    for (int i = 0; i < n_medio; ++i) {
        claves_aux[pos++] = medio->claves_[i];
    }

    // Copiamos la segunda clave separadora del padre.
    claves_aux[pos++] = padre->claves_[indice + 1];

    // Copiamos claves del hijo derecho.
    for (int i = 0; i < n_der; ++i) {
        claves_aux[pos++] = der->claves_[i];
    }

    // Arreglo temporal para juntar los hijos internos.
    // Solo se usa si los nodos no son hojas.
    Nodo<T> **hijos_aux = nullptr;
    int total_hijos = 0;

    if (!son_hojas) {
        // Cada nodo interno con k claves tiene k + 1 hijos.
        total_hijos = (n_izq + 1) + (n_medio + 1) + (n_der + 1);

        hijos_aux = new Nodo<T>*[total_hijos];

        int pos_h = 0;

        // Copiamos hijos del nodo izquierdo.
        for (int i = 0; i <= n_izq; ++i) {
            hijos_aux[pos_h++] = izq->hijos_[i];
        }

        // Copiamos hijos del nodo medio.
        for (int i = 0; i <= n_medio; ++i) {
            hijos_aux[pos_h++] = medio->hijos_[i];
        }

        // Copiamos hijos del nodo derecho.
        for (int i = 0; i <= n_der; ++i) {
            hijos_aux[pos_h++] = der->hijos_[i];
        }
    }

    // En B* 3->2:
    // de todas las claves juntadas, una se queda en el padre.
    int claves_a_repartir = total_claves - 1;

    // El nodo izquierdo recibe la mitad inferior.
    int claves_izq = claves_a_repartir / 2;

    // La clave inmediatamente después de las claves del izquierdo
    // será la nueva separadora del padre.
    int indice_separadora = claves_izq;

    // El nodo medio recibe el resto de las claves.
    // El nodo derecho será eliminado.
    int claves_medio = claves_a_repartir - claves_izq;

    // Pasamos claves al nodo izquierdo.
    for (int i = 0; i < claves_izq; ++i) {
        izq->claves_[i] = claves_aux[i];
    }

    // Actualizamos cuántas claves tiene el nodo izquierdo.
    izq->num_claves_ = claves_izq;

    // Actualizamos la clave separadora del padre.
    padre->claves_[indice] = claves_aux[indice_separadora];

    // Pasamos claves al nodo medio.
    // Empezamos después de la clave separadora.
    for (int i = 0; i < claves_medio; ++i) {
        medio->claves_[i] = claves_aux[indice_separadora + 1 + i];
    }

    // Actualizamos cuántas claves tiene el nodo medio.
    medio->num_claves_ = claves_medio;

    // Si no son hojas, también debemos redistribuir los hijos.
    if (!son_hojas) {
        // El nodo izquierdo necesita claves_izq + 1 hijos.
        for (int i = 0; i <= claves_izq; ++i) {
            izq->hijos_[i] = hijos_aux[i];

            // Cada hijo debe saber quién es su nuevo padre.
            if (izq->hijos_[i] != nullptr) {
                izq->hijos_[i]->padre_ = izq;
            }
        }

        // El nodo medio necesita claves_medio + 1 hijos.
        for (int i = 0; i <= claves_medio; ++i) {
            medio->hijos_[i] = hijos_aux[claves_izq + 1 + i];

            // Cada hijo debe saber quién es su nuevo padre.
            if (medio->hijos_[i] != nullptr) {
                medio->hijos_[i]->padre_ = medio;
            }
        }

        // Limpiamos punteros sobrantes del nodo izquierdo.
        // Esto evita dejar hijos viejos apuntando a subárboles incorrectos.
        for (int i = claves_izq + 1; i <= orden_; ++i) {
            izq->hijos_[i] = nullptr;
        }

        // Limpiamos punteros sobrantes del nodo medio.
        for (int i = claves_medio + 1; i <= orden_; ++i) {
            medio->hijos_[i] = nullptr;
        }
    }

    // Ahora debemos borrar del padre la segunda clave separadora.
    // La primera fue reemplazada por la nueva separadora.
    for (int i = indice + 1; i < padre->num_claves_ - 1; ++i) {
        padre->claves_[i] = padre->claves_[i + 1];
    }

    // También debemos borrar el puntero al hijo derecho.
    // El hijo derecho ya fue absorbido por izq y medio.
    for (int i = indice + 2; i < padre->num_claves_; ++i) {
        padre->hijos_[i] = padre->hijos_[i + 1];
    }

    // Limpiamos el último puntero que quedó repetido o colgante.
    padre->hijos_[padre->num_claves_] = nullptr;

    // El padre perdió una clave, porque dos separadoras se volvieron una.
    padre->num_claves_--;

    // Ya no necesitamos el nodo derecho.
    delete der;

    // Liberamos memoria temporal.
    delete[] claves_aux;
    delete[] hijos_aux;
}

// ------------------------------------------------------- //
/** \brief Divide la raíz cuando ocurre overflow en ella. */
template <typename T>
void ArbolBEstrella<T>::dividirRaiz(Nodo<T> *vieja_raiz)
{
    try
    {
        //Crear un nuevo nodo, que sera la nueva raiz
        Nodo<T> *nueva_raiz = new Nodo<T>(orden_, false);

        //Crear un nuevo nodo que sera el hermano de la vieja raiz
        Nodo<T> *hno_vieja_raiz = new Nodo<T>(orden_, vieja_raiz->es_hoja_);

        //Calcular en que índice del arreglo de claves de la vieja raíz esta la clave que debe subir a la nueva raíz
        int claves_repartir = vieja_raiz->num_claves_ - 1;
        int num_claves_vieja_raiz = claves_repartir / 2;
        int num_claves_hno_vieja_raiz = claves_repartir - num_claves_vieja_raiz;
        int indice_clave_nueva_raiz = num_claves_vieja_raiz;

        //Mudar las claves y conectar los hijos

        //Mudar la clave del padre y los hijos
        nueva_raiz->claves_[0] = vieja_raiz->claves_[indice_clave_nueva_raiz];
        nueva_raiz->num_claves_ = 1;
        nueva_raiz->hijos_[0] = vieja_raiz;
        vieja_raiz->padre_ = nueva_raiz;
        nueva_raiz->hijos_[1] = hno_vieja_raiz;
        hno_vieja_raiz->padre_ = nueva_raiz;

        //Mudar las claves al hermano
        for(int i = 0; i < num_claves_hno_vieja_raiz; ++i){
            hno_vieja_raiz->claves_[i] = vieja_raiz->claves_[indice_clave_nueva_raiz + 1 + i];
        }
        hno_vieja_raiz->num_claves_ = num_claves_hno_vieja_raiz;

        //Si la raiz no es una hoja, entonces mudar a los hijos
        if(vieja_raiz->es_hoja_ == false){
            for(int i = 0; i < (num_claves_hno_vieja_raiz + 1); ++i){
                hno_vieja_raiz->hijos_[i] = vieja_raiz->hijos_[indice_clave_nueva_raiz + 1 + i];
                hno_vieja_raiz->hijos_[i]->padre_ = hno_vieja_raiz;
            }
        }
        vieja_raiz->num_claves_ = num_claves_vieja_raiz;
        raiz_ = nueva_raiz;

    }
    catch(std::bad_alloc &error)
    {
        throw MemoriaInsuficiente();
    }
}

// ============================================================================
//  Eliminación y corrección de deficiencias
// ============================================================================

/** \brief Elimina una clave dentro de un subárbol. */
template<typename T>
bool ArbolBEstrella<T>::eliminar(Nodo<T> *subraiz, T clave) {
    // Si llegamos a un nodo nulo, la clave no existe.
    if (subraiz == nullptr) {
        return false;
    }

    int i = 0;

    // Buscamos la primera posición donde clave <= subraiz->claves_[i].
    while (i < subraiz->num_claves_ && clave > subraiz->claves_[i]) {
        i++;
    }

    // ============================================================
    // Caso 1: la clave sí está dentro de este nodo.
    // ============================================================
    if (i < subraiz->num_claves_ && clave == subraiz->claves_[i]) {

        // ------------------------------------------------------------
        // Caso 1A: la clave está en una hoja.
        // ------------------------------------------------------------
        if (subraiz->es_hoja_) {
            // Recorremos las claves hacia la izquierda para tapar el hueco.
            for (int j = i; j < subraiz->num_claves_ - 1; ++j) {
                subraiz->claves_[j] = subraiz->claves_[j + 1];
            }

            // El nodo perdió una clave.
            subraiz->num_claves_--;

            // Si la raíz quedó vacía, la ajustamos.
            ajustarRaizDespuesDeEliminar();

            return true;
        }

        // ------------------------------------------------------------
        // Caso 1B: la clave está en un nodo interno.
        // ------------------------------------------------------------
        Nodo<T> *hijo_izq = subraiz->hijos_[i];
        Nodo<T> *hijo_der = subraiz->hijos_[i + 1];

        int minimo = minimoClaves();

        // Si el hijo izquierdo tiene claves extra,
        // reemplazamos la clave por su predecesor.
        if (hijo_izq != nullptr && hijo_izq->num_claves_ > minimo) {
            // El predecesor es la clave más grande del subárbol izquierdo.
            T pred = obtenerPredecesor(subraiz, i);

            // Subimos el predecesor al lugar de la clave eliminada.
            subraiz->claves_[i] = pred;

            // Ahora eliminamos el predecesor desde el hijo izquierdo.
            bool resultado = eliminar(hijo_izq, pred);

            // Si después de eliminar el hijo izquierdo quedó deficiente,
            // corregimos el underflow desde este nodo.
            if (resultado && esDeficiente(subraiz->hijos_[i])) {
                corregirDeficiencia(subraiz, i);
            }

            // Ajustamos la raíz si fue necesario.
            ajustarRaizDespuesDeEliminar();

            return resultado;
        }

        // Si el hijo izquierdo no puede donar,
        // usamos el sucesor desde el subárbol derecho.
        else {
            // El sucesor es la clave más pequeña del subárbol derecho.
            T suc = obtenerSucesor(subraiz, i);

            // Subimos el sucesor al lugar de la clave eliminada.
            subraiz->claves_[i] = suc;

            // Ahora eliminamos el sucesor desde el hijo derecho.
            bool resultado = eliminar(hijo_der, suc);

            // Si después de eliminar el hijo derecho quedó deficiente,
            // corregimos el underflow desde este nodo.
            if (resultado && esDeficiente(subraiz->hijos_[i + 1])) {
                corregirDeficiencia(subraiz, i + 1);
            }

            // Ajustamos la raíz si fue necesario.
            ajustarRaizDespuesDeEliminar();

            return resultado;
        }
    }

    // ============================================================
    // Caso 2: la clave no está en este nodo.
    // ============================================================

    // Si este nodo es hoja y no encontramos la clave,
    // entonces la clave no existe en el árbol.
    if (subraiz->es_hoja_) {
        return false;
    }

    // Bajamos al hijo donde debería estar la clave.
    bool resultado = eliminar(subraiz->hijos_[i], clave);

    // Si sí se eliminó algo y el hijo quedó deficiente,
    // corregimos el underflow.
    if (resultado && esDeficiente(subraiz->hijos_[i])) {
        corregirDeficiencia(subraiz, i);
    }

    // La raíz pudo haber quedado vacía después de corregir.
    ajustarRaizDespuesDeEliminar();

    return resultado;
}

// ------------------------------------------------------- //
/** \brief Calcula el mínimo de claves requerido para nodos no raíz. */
template <typename T>
int ArbolBEstrella<T>::minimoClaves() const {
    int minimo = (2 * orden_ - 1) / 3;

    if (minimo < 1) {
        minimo = 1;
    }

    return minimo;
}

// ------------------------------------------------------- //
/** \brief Determina si un nodo no raíz tiene menos claves que el mínimo permitido. */
template <typename T>
bool ArbolBEstrella<T>::esDeficiente(Nodo<T> *nodo) const {
    if (nodo == raiz_) return false;
    return nodo->num_claves_ < minimoClaves();
}

// ------------------------------------------------------- //
/** \brief Obtiene el sucesor de una clave interna. */
template <typename T>
T ArbolBEstrella<T>::obtenerSucesor(Nodo<T> *nodo, int indice) const {
    Nodo<T> *actual = nodo->hijos_[indice + 1];
    while (!actual->es_hoja_) actual = actual->hijos_[0];

    return actual->claves_[0];
}

// ------------------------------------------------------- //
/** \brief Obtiene el predecesor de una clave interna. */
template <typename T>
T ArbolBEstrella<T>::obtenerPredecesor(Nodo<T> *nodo, int indice) const {
    Nodo<T> *actual = nodo->hijos_[indice];
    while (!actual->es_hoja_) actual = actual->hijos_[actual->num_claves_];

    return actual->claves_[actual->num_claves_ - 1];
}

// ------------------------------------------------------- //
/** \brief Repara el underflow de un hijo mediante redistribución o fusión. */
template <typename T>
void ArbolBEstrella<T>::corregirDeficiencia(Nodo<T> *nodo_padre, int indice_hijo) {
    // Si no hay padre, no podemos corregir desde aquí.
    if (nodo_padre == nullptr) {
        return;
    }

    // indice_hijo debe ser un índice válido dentro de los hijos del padre.
    // Un nodo con k claves tiene k + 1 hijos, por eso puede llegar hasta num_claves_.
    if (indice_hijo < 0 || indice_hijo > nodo_padre->num_claves_) {
        return;
    }

    // Nodo que quedó con menos claves de las permitidas.
    Nodo<T> *hijo_deficiente = nodo_padre->hijos_[indice_hijo];

    // Si no existe o realmente no está deficiente, no hacemos nada.
    if (hijo_deficiente == nullptr || !esDeficiente(hijo_deficiente)) {
        return;
    }

    // Mínimo de claves que debe conservar un nodo no raíz.
    int minimo = minimoClaves();

    // ============================================================
    // 1. Intentar pedir prestado al hermano izquierdo.
    // ============================================================
    if (indice_hijo > 0) {
        Nodo<T> *hermano_izq = nodo_padre->hijos_[indice_hijo - 1];

        // Solo podemos pedir prestado si el hermano tiene más del mínimo.
        // Si tiene exactamente el mínimo, no le podemos quitar una clave.
        if (hermano_izq != nullptr && hermano_izq->num_claves_ > minimo) {
            // Redistribuimos entre hermano izquierdo y nodo deficiente.
            redistribuir(nodo_padre, hermano_izq, hijo_deficiente, indice_hijo - 1);
            return;
        }
    }

    // ============================================================
    // 2. Intentar pedir prestado al hermano derecho.
    // ============================================================
    if (indice_hijo < nodo_padre->num_claves_) {
        Nodo<T> *hermano_der = nodo_padre->hijos_[indice_hijo + 1];

        // Igual que antes: solo puede prestar si tiene más del mínimo.
        if (hermano_der != nullptr && hermano_der->num_claves_ > minimo) {
            // Redistribuimos entre nodo deficiente y hermano derecho.
            redistribuir(nodo_padre, hijo_deficiente, hermano_der, indice_hijo);
            return;
        }
    }

    // ============================================================
    // 3. Si no se pudo prestar, intentamos fusión B*: 3 hijos -> 2 hijos.
    // ============================================================
    if (nodo_padre->num_claves_ >= 2) {
        int indice_fusion = 0;

        // Si el hijo deficiente es el primero,
        // usamos la ventana de hijos 0, 1 y 2.
        if (indice_hijo == 0) {
            indice_fusion = 0;
        }

        // Si el hijo deficiente es el último,
        // usamos la última ventana posible de tres hijos.
        else if (indice_hijo == nodo_padre->num_claves_) {
            indice_fusion = nodo_padre->num_claves_ - 2;
        }

        // Si está en medio,
        // usamos hijo izquierdo, hijo deficiente y hijo derecho.
        else {
            indice_fusion = indice_hijo - 1;
        }

        // Aquí por fin usamos la función fusionar.
        // Esta es la operación B* importante.
        fusionar(nodo_padre, indice_fusion);
        return;
    }

    // ============================================================
    // 4. Caso especial: el padre solo tiene una clave.
    // ============================================================
    // Si el padre tiene una sola clave, solo tiene dos hijos.
    // Entonces no hay tres hijos disponibles para hacer B* 3->2.
    // En ese caso usamos fusión clásica 2->1.

    if (indice_hijo > 0) {
        // Fusionamos hermano izquierdo + hijo deficiente.
        fusionarDosHijos(nodo_padre, indice_hijo - 1);
    }
    else {
        // Fusionamos hijo deficiente + hermano derecho.
        fusionarDosHijos(nodo_padre, indice_hijo);
    }
}

// ------------------------------------------------------- //
/** \brief Fusiona dos hijos usando la clave separadora del padre. */
template <typename T>
void ArbolBEstrella<T>::fusionarDosHijos(Nodo<T> *nodo_padre, int indice_hijo_izq) {
    // Validamos que el padre exista.
    if (nodo_padre == nullptr) {
        return;
    }

    // indice_hijo_izq debe apuntar a una clave separadora válida.
    // Esa clave separa a hijos_[indice_hijo_izq] e hijos_[indice_hijo_izq + 1].
    if (indice_hijo_izq < 0 || indice_hijo_izq >= nodo_padre->num_claves_) {
        return;
    }

    // Tomamos los dos hijos que se van a fusionar.
    Nodo<T> *izq = nodo_padre->hijos_[indice_hijo_izq];
    Nodo<T> *der = nodo_padre->hijos_[indice_hijo_izq + 1];

    // Si alguno no existe, no se puede fusionar.
    if (izq == nullptr || der == nullptr) {
        return;
    }

    // Total de claves del nuevo nodo:
    // claves del izquierdo + separadora del padre + claves del derecho.
    int total_claves = izq->num_claves_ + 1 + der->num_claves_;

    // Si no cabe en un solo nodo, no hacemos merge porque habría overflow.
    // En ese caso redistribuimos como fallback seguro.
    if (total_claves > max_claves_) {
        redistribuir(nodo_padre, izq, der, indice_hijo_izq);
        return;
    }

    // pos empieza al final de las claves actuales del izquierdo.
    int pos = izq->num_claves_;

    // Bajamos la clave separadora del padre al nodo izquierdo.
    izq->claves_[pos++] = nodo_padre->claves_[indice_hijo_izq];

    // Copiamos todas las claves del nodo derecho al izquierdo.
    for (int i = 0; i < der->num_claves_; ++i) {
        izq->claves_[pos++] = der->claves_[i];
    }

    // Si no son hojas, también debemos mover los hijos del derecho.
    if (!izq->es_hoja_) {
        // El primer hijo del derecho se coloca después de los hijos actuales del izquierdo.
        int inicio_hijos = izq->num_claves_ + 1;

        for (int i = 0; i <= der->num_claves_; ++i) {
            izq->hijos_[inicio_hijos + i] = der->hijos_[i];

            // Actualizamos el padre de cada hijo movido.
            if (izq->hijos_[inicio_hijos + i] != nullptr) {
                izq->hijos_[inicio_hijos + i]->padre_ = izq;
            }
        }
    }

    // El izquierdo ahora contiene todas las claves fusionadas.
    izq->num_claves_ = total_claves;

    // Quitamos del padre la clave separadora que bajó al hijo.
    for (int i = indice_hijo_izq; i < nodo_padre->num_claves_ - 1; ++i) {
        nodo_padre->claves_[i] = nodo_padre->claves_[i + 1];
    }

    // Quitamos del padre el puntero al hijo derecho eliminado.
    for (int i = indice_hijo_izq + 1; i < nodo_padre->num_claves_; ++i) {
        nodo_padre->hijos_[i] = nodo_padre->hijos_[i + 1];
    }

    // Limpiamos el último puntero sobrante.
    nodo_padre->hijos_[nodo_padre->num_claves_] = nullptr;

    // El padre perdió una clave.
    nodo_padre->num_claves_--;

    // El nodo derecho ya fue absorbido.
    delete der;
}

// ------------------------------------------------------- //
/** \brief Reemplaza o elimina la raíz cuando queda vacía. */
template <typename T>
void ArbolBEstrella<T>::ajustarRaizDespuesDeEliminar() {
    // Si no hay raíz, no hay nada que ajustar.
    if (raiz_ == nullptr) {
        return;
    }

    // Si la raíz todavía tiene claves, está bien.
    if (raiz_->num_claves_ > 0) {
        return;
    }

    // Si la raíz quedó vacía y además era hoja,
    // entonces el árbol completo quedó vacío.
    if (raiz_->es_hoja_) {
        delete raiz_;
        raiz_ = nullptr;
        return;
    }

    // Si la raíz quedó vacía pero tiene hijos,
    // el hijo 0 sube y se convierte en la nueva raíz.
    Nodo<T> *vieja_raiz = raiz_;
    raiz_ = raiz_->hijos_[0];

    // Cortamos el enlace para que la vieja raíz ya no apunte al nuevo árbol.
    vieja_raiz->hijos_[0] = nullptr;

    // La nueva raíz no debe tener padre.
    if (raiz_ != nullptr) {
        raiz_->padre_ = nullptr;
    }

    // Borramos la raíz vieja.
    delete vieja_raiz;
}

// ============================================================================
//  Impresión
// ============================================================================

/** \brief Imprime recursivamente un subárbol por niveles. */
template <typename T>
void ArbolBEstrella<T>::imprimir(Nodo<T> *nodo, int nivel) const {
    if (nodo == nullptr) return;

    for (int i = nodo->num_claves_ - 1; i >= 0; --i) {
        imprimir(nodo->hijos_[i + 1], nivel + 1);
        for (int j = 0; j < nivel; ++j) std::cout << "    ";
        std::cout << nodo->claves_[i] << std::endl;
    }

    imprimir(nodo->hijos_[0], nivel + 1);
}

