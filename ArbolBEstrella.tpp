#include "ArbolBEstrella.hpp"
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

    //Guardamos los conteos originales de claves antes de modificarlos,
    //los necesitamos para recolectar correctamente los hijos más adelante
    int n_izq = izq->num_claves_;
    int n_medio = medio->num_claves_;
    int n_der = der->num_claves_;

    // Guardamos todas las claves(del padre y de los hijos) en un arreglo epicamente
    T *temp = new T[3 * orden_ - 1]; //Formula para saber el tamaño del arreglo epica
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
    Nodo<T> **temp_hijos = new Nodo<T> *[3 * orden_]; //El tamaño del arreglo es 1 más
    pos = 0;
    j = 0;

    // Metemos todos los punteros a hijos en un arreglo
    // Usamos los conteos ORIGINALES porque izq y medio aún conservan sus hijos viejos

    // todo del lado izquierdo
    for (int i = 0; i <= n_izq; ++i) {
        temp_hijos[pos++] = izq->hijos_[i];
    }

    // todo de en medio
    for (int i = 0; i <= n_medio; ++i) {
        temp_hijos[pos++] = medio->hijos_[i];
    }

    // todo del lado derecho
    for (int i = 0; i <= n_der; ++i) {
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

    //Liberamos los arreglos temporales que reservamos dinámicamente
    delete[] temp;
    delete[] temp_hijos;

}

//METODOS DE CLASES AUXILIARES
template <typename T>
ArbolBEstrella<T>::MemoriaInsuficiente::MemoriaInsuficiente() throw(){}
//**

//Método que informa cual fue el prolema
template <typename T>
const char* ArbolBEstrella<T>::MemoriaInsuficiente::what() const throw()
{
    return "Memoria insuficiente";
}

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

//**

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

//**

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

//***************************************************************

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

//***************************************************************

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

//***********************************************************************

template <typename T>
void ArbolBEstrella<T>::redistribuir(Nodo<T> *nodo_padre, Nodo<T> *nodo_hijo_izq, Nodo<T> *nodo_hijo_der, int indice_nodo_hijo_izq)
{
    try
    {
        //Crear arreglo auxiliar para claves
        int num_claves_nodo_hijo_izq = nodo_hijo_izq->num_claves_;
        int num_claves_nodo_hijo_der = nodo_hijo_der->num_claves_;
        int tam_arreglo_aux = num_claves_nodo_hijo_izq + num_claves_nodo_hijo_der + 1;

        //Crear arreglo auxiliar para claves
        T *arreglo_aux = new T[tam_arreglo_aux];

        //Si los nodos tienen hijos, crear un arreglo auxiliar para los hijos
        Nodo<T> **arreglo_hijos_aux = nullptr;
        int tam_arreglo_aux_hijos = tam_arreglo_aux + 1;
        if(nodo_hijo_izq->es_hoja_ == false){
            arreglo_hijos_aux = new Nodo<T>*[tam_arreglo_aux_hijos];
        }

        //PONER LAS CLAVES Y LOS HIJOS DE LOS NODOS HIJOS Y DEL NODO PADRE EN ARREGLOS AUXILIARES

        //Poner las claves del hijo izquierdo en el arreglo auxiliar
        for(int i = 0; i < num_claves_nodo_hijo_izq; ++i){
            arreglo_aux[i] = nodo_hijo_izq->claves_[i];
        }

        //Poner la clave separadora del padre en el medio del arreglo auxiliar
        arreglo_aux[num_claves_nodo_hijo_izq] = nodo_padre->claves_[indice_nodo_hijo_izq];

        //Poner las claves del hijo derecho en el arreglo auxiliar
        int ultimo_indice_arreglo_aux = num_claves_nodo_hijo_izq;
        for(int i = 0; i < num_claves_nodo_hijo_der; ++i){
            arreglo_aux[ultimo_indice_arreglo_aux + 1 + i] = nodo_hijo_der->claves_[i];
        }

        //Si los nodos tienen hijos, extraerlos también
        if(nodo_hijo_izq->es_hoja_ == false){
            // Agregar los hijos del nodo izquierdo
            for(int i = 0; i < (num_claves_nodo_hijo_izq + 1); ++i){
                arreglo_hijos_aux[i] = nodo_hijo_izq->hijos_[i];
            }
            // Agregar los hijos del nodo derecho
            for(int i = 0; i < (num_claves_nodo_hijo_der + 1); ++i){
                arreglo_hijos_aux[num_claves_nodo_hijo_izq + 1 + i] = nodo_hijo_der->hijos_[i];
            }
        }

        //CALCULAR CUANTAS CLAVES E HIJOS TENDRA CADA NODO HIJO

        //Calcular cuántas claves tendrá cada nodo hermano
        int num_claves_a_repartir = tam_arreglo_aux - 1;
        int num_claves_nuevo_izq = num_claves_a_repartir / 2;
        int num_claves_nuevo_der = num_claves_a_repartir - num_claves_nuevo_izq;

        //Calcular el índice de la clave que se quedará en el padre funcionando como separadora
        int indice_separadora = num_claves_nuevo_izq;

        //REPARTIR LAS CLAVES A LOS NODOS HIJOS 50/50

        //Mudar las claves que van en el nodo izquierdo
        for(int i = 0; i < num_claves_nuevo_izq; ++i){
            nodo_hijo_izq->claves_[i] = arreglo_aux[i];
        }

        //Mudar los hijos que van en el nodo izquierdo
        if(nodo_hijo_izq->es_hoja_ == false){
            for(int i = 0; i < (num_claves_nuevo_izq + 1); ++i){
                nodo_hijo_izq->hijos_[i] = arreglo_hijos_aux[i];
                nodo_hijo_izq->hijos_[i]->padre_ = nodo_hijo_izq;
            }
        }

        //Poner la clave separadora en el padre
        nodo_padre->claves_[indice_nodo_hijo_izq] = arreglo_aux[indice_separadora];

        //Mudar las claves que van en el nodo derecho
        for(int i = 0; i < num_claves_nuevo_der; ++i){
            nodo_hijo_der->claves_[i] = arreglo_aux[indice_separadora + 1 + i];
        }

        //Mudar los hijos que van en el nodo derecho
        if(nodo_hijo_izq->es_hoja_ == false){
            for(int i = 0; i < (num_claves_nuevo_der + 1); ++i){
                nodo_hijo_der->hijos_[i] = arreglo_hijos_aux[indice_separadora + 1 + i];
                nodo_hijo_der->hijos_[i]->padre_ = nodo_hijo_der;
            }
        }

        //Actualizar los contadores de las claves
        nodo_hijo_izq->num_claves_ = num_claves_nuevo_izq;
        nodo_hijo_der->num_claves_ = num_claves_nuevo_der;

        //Liberar memoria
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

//***********************************************************************

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