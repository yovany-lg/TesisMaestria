/* 
 * File:   nDEVM.cpp
 * Author: yova
 * 
 * Created on 12 de agosto de 2014, 03:29 PM
 */
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "nDEVM.h"

template <typename T> std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

nDEVM::nDEVM() {
    rootNode = NULL;
    resetCoupletIndex();
}

nDEVM::nDEVM(trieNode *node) {
    rootNode = node;
    resetCoupletIndex();
}

nDEVM::nDEVM(const nDEVM& orig) {
}

nDEVM::~nDEVM() {   //METODO PARA ELIMINAR EL ARBOL TRIE
}

void nDEVM::resetCoupletIndex(){
    coupletIndex = &rootNode;
}

bool nDEVM::isEmpty(){
    if(rootNode == NULL)
        return true;
    else
        return false;
}

trieNode *nDEVM::getRootNode(){
    return rootNode;
}

/*Método Auxiliar para Insertar un vértice en un árbol trie.
 * Argumentos:
 * -Nodo raíz.
 * -Apuntador al vector de entrada
 * -Tamaño del vector.
 */
void nDEVM::insertVertex(double * inputKey,int length){
    trieNode * prevNode = NULL;
    insertVertex(&prevNode,&rootNode,inputKey,length,0,0,length);
}

/*Método Auxiliar para Insertar un vértice en un árbol trie.
 * Argumentos:
 * -Nodo raíz.
 * -Apuntador al vector de entrada
 * -Tamaño del vector.
 */
void nDEVM::insertVertex(trieNode **otherRootNode,double * inputKey,int length){
    trieNode * prevNode = NULL;
    insertVertex(&prevNode,otherRootNode,inputKey,length,0,0,length);
}

/**
 * Método Principal Recursivo para Insertar un vértice en un árbol trie. 
 * Se utilizan contadores para determinar si se alcanzó la profundidad máxima.
 * @param prevNode: Nodo Trie previo en la exploracion.
 * @param currentNode: Nodo Trie actual en la exploracion.
 * @param inputKey: Arreglo del vertice que se va a insertar.
 * @param length: Tamaño maximo de el arreglo del vertice.
 * @param prevDim: Dimension previa en la exploracion.
 * @param currentDim: Dimension actual en la exploracion.
 * @param matchCount: Contador de coincidencias en el Trie, usado para saber si
 * el vertice que se va a insertar ya existe.
 * @return @return Se retorna un valor booleano que indica si el nodo anterior puede 
 * ser eliminado
 */
bool nDEVM::insertVertex(trieNode **prevNode,trieNode **currentNode,double * inputKey,
        int length,int prevDim,int currentDim,int matchCount){
    //currentDim inicia desde cero, es la posicion de la dimension actual en el array
    if(!(currentDim < length))
        if(matchCount == 0)
            return true;   //El vertice ya existe
        else
            return false;    //El vertice es nuevo
    
    if(*currentNode == NULL){
        trieNode *node = new trieNode;
        node->value = inputKey[currentDim];
        node->nextDim = NULL;
        node->nextTrieNode = NULL;
        
        *currentNode = node;
        //Que no sea el nodo raiz
        if(*prevNode != NULL)
            //Cuando se inserta un nodo en la misma dimension
            if(prevDim == currentDim)
                (*prevNode)->nextTrieNode = *currentNode;
            else
                (*prevNode)->nextDim = *currentNode;
        //Moverse a la siguiente dimension
        insertVertex(currentNode,&((*currentNode)->nextDim),inputKey,length,currentDim,currentDim+1,matchCount);
    }else{
        //Si ya existe un nodo con el mismo valor en esta dimension
        if((*currentNode)->value == inputKey[currentDim]){
            bool vertexStatus = insertVertex(currentNode,&((*currentNode)->nextDim),inputKey,length,currentDim,currentDim+1,matchCount-1);
            //Si el vertice ya existe, se intenta eliminar
            if(vertexStatus){
                //Si es el nodo raiz
                if(rootNode == (*currentNode)){
                    //Si existe un nodo siguiente en la primer dimension
                    if(rootNode->nextTrieNode != NULL){
                        trieNode *tempNode = rootNode;
                        rootNode = rootNode->nextTrieNode;
                        delete tempNode;
                        tempNode = NULL;
                    }else{
                        rootNode == NULL;
                        delete *currentNode;
                        *currentNode = NULL;
                    }
                }else{
                    
                    //Si se llega por profundidad
                    if((*prevNode)->nextDim == (*currentNode))
                    {
                        //Si no hay mas nodos a la derecha
                        if((*currentNode)->nextTrieNode == NULL)
                        {
                            delete *currentNode;
                            *currentNode = NULL;
                            //Intentar eliminar nodo padre
                            return true;
                        }else
                        {
                            trieNode *node = (*currentNode)->nextTrieNode;
                            delete *currentNode;
                            *currentNode = NULL;
                            (*prevNode)->nextDim = node;
                            //Nodo padre no puede ser eliminado
                            return false;
                        }
                    }else
                    {
                        //Se llega por amplitud, busqueda en la misma dimension
                        if((*currentNode)->nextTrieNode == NULL)
                        {
                            delete *currentNode;
                            *currentNode = NULL;
                        }else
                        {
                            trieNode *node = (*currentNode)->nextTrieNode;
                            delete *currentNode;
                            *currentNode = NULL;
                            (*prevNode)->nextTrieNode= node;
                        }
                        //Nodo padre no puede ser eliminado
                        return false;
                    }                
                }
            }
        }else    
            //Busqueda por comparacion
            if((*currentNode)->value > inputKey[currentDim]){
                //Si hay un nodo en la misma dimension con un valor mayor
                //Insertar nuevo nodo antes de dicho nodo
                trieNode *node = new trieNode;
                node->value = inputKey[currentDim];
                node->nextDim = NULL;
                node->nextTrieNode = (*currentNode);
                //Si es el nodo raiz
                if(rootNode == (*currentNode))
                    rootNode = node;
                else{
                    //Si se llega por profundidad
                    if((*prevNode)->nextDim == (*currentNode)){
                        (*prevNode)->nextDim = node;                
                    }else{
                        (*prevNode)->nextTrieNode = node;                
                    }
                }                

                insertVertex(&node,&node->nextDim,inputKey,length,currentDim,currentDim+1,matchCount);
            }else
                //Se explora otro nodo en la misma dimension
                insertVertex(currentNode,&((*currentNode)->nextTrieNode),inputKey,length,currentDim,currentDim,matchCount);
    }
}

/**
 * Método Auxiliar para Imprimir en consola el contenido de un árbol Trie.
 */
void nDEVM::printTrie(){
    double * testKey = new double[4];
    printTrie(rootNode,&testKey,0);
}

/**
 * Método Principal Recursivo para Imprimir en consola el contenido de un árbol Trie.
 * @param currentNode, Nodo Trie actual en la exploración.
 * @param key: Vector auxiliar para almacenar los vértices hallados en el Trie.
 * @param dim: Dimensión actual en el la exploración.
 */
void nDEVM::printTrie(trieNode *currentNode,double **key, int dim){
    if((currentNode) == NULL){   //que el arbol no este vacio
        cout << vectorToString(key,dim)<<endl;
        return;
    }

    (*key)[dim] = (currentNode)->value;
    printTrie((currentNode)->nextDim, key, dim+1);

    if((currentNode)->nextTrieNode != NULL){
        printTrie((currentNode)->nextTrieNode, key, dim);
    }
}

/*Método Auxiliar para obtener el Tamaño de un árbol trie.
 * Argumentos:
 * -Nodo raíz.
*/
double nDEVM::EVMSize(){
    double size = 0.0;
    if((&rootNode) == NULL){   //que el arbol no este vacio
        return 0.0;
    }
    EVMSize(rootNode,&size);
    return size;
}

/**
 * Método Principal para obtener el Tamaño (número de nodos hoja) de un árbol Trie.
 * @param currentNode: Nodo actual en la exploración.
 * @param size: Contador del tamaño.
 */
void nDEVM::EVMSize(trieNode *currentNode,double *size){
    //Si se llega a un nodo hoja
    if((currentNode) == NULL){
        *size = (*size) +1;
        return;
    }

    EVMSize((currentNode)->nextDim,size);

    if((currentNode)->nextTrieNode != NULL){
        EVMSize((currentNode)->nextTrieNode,size);
    }
}

/*Método Auxiliar para comparar dos árboles trie.
 * Argumentos:
 * -Nodo raíz del primer arbol.
 * -Nodo raíz del segundo arbol.
*/
/**
 * Método para comparar dos EVMs.
 * @param otherEVM: El EVM que será comparado con el EVM actual.
 * @return Se retorna un valor booleano (true, false).
 */
bool nDEVM::compareEVM(nDEVM *otherEVM){
    bool compare1 = true, compare2 = true;
    compareTrie(&rootNode,&(otherEVM->rootNode),&compare1);
    compareTrie(&(otherEVM->rootNode),&rootNode,&compare2);
    return compare1 and compare2;
}

/**
 * Método para comparar dos EVMs considerando los couplets sobre la primera dimensión
 * @param otherEVM: El EVM que será comparado con el EVM actual.
 * @return Se retorna un valor booleano (true, false).
 */
bool nDEVM::compareByCouplets(nDEVM *otherEVM){
    bool compare1, compare2, totalCompare;
    nDEVM* currentCouplet1 = new nDEVM();
    nDEVM* currentCouplet2 = new nDEVM();
    
    while(!(endEVM()) and !(otherEVM->endEVM())){
        compare1 = compare2 = true;
        currentCouplet1 = readCouplet();
        currentCouplet2 = otherEVM->readCouplet();
        compareTrie(&(currentCouplet1->rootNode),&(currentCouplet2->rootNode),&compare1);
        compareTrie(&(currentCouplet2->rootNode),&(currentCouplet1->rootNode),&compare2);
        totalCompare = compare1 and compare2;
        if(!totalCompare)
            return false;
    }
    
    if(endEVM()){
        if(!(otherEVM->endEVM())){
            resetCoupletIndex();
            otherEVM->resetCoupletIndex();
            return false;
        }
    }else{
        if(otherEVM->endEVM()){
            resetCoupletIndex();
            otherEVM->resetCoupletIndex();
            return false;
        }
    }
    
    resetCoupletIndex();
    otherEVM->resetCoupletIndex();
    return totalCompare;
}

/**
 * Método Principal para comparar dos árboles Trie.
 * @param currentNode: Nodo actual del primer árbol Trie en la exploración.
 * @param otherCurrentNode: Nodo actual del segundo árbol Trie en la exploración.
 * @param compare: Par almacenar la comparación.
 */
void nDEVM::compareTrie(trieNode **currentNode,trieNode **otherCurrentNode,bool *compare){
    //Si en alguna llamada son diferentes
    if((*compare) == false)
        return;
    
    //Si en alguna llamada ambos árboles estan vacíos
    if((*currentNode) == NULL and (*otherCurrentNode) == NULL){
        (*compare) = true;
        return;
    }else{        
        if((*currentNode)->value != (*otherCurrentNode)->value){
            (*compare) = false;
            return;
        }
        //Si no es nodo hoja, explorar la siguiente dimension
        if((*currentNode)->nextDim != NULL){
            if((*otherCurrentNode)->nextDim != NULL)
                compareTrie(&(*currentNode)->nextDim,&(*otherCurrentNode)->nextDim,compare);
            else{
                (*compare) = false;
                return;
            }
        }else//Es nulo en el primer trie
            if((*otherCurrentNode)->nextDim != NULL){ //Si no es nulo el otro trie
                (*compare) = false;
                return;
            }
        
        if((*compare) == false)
            return;

        if((*currentNode)->nextTrieNode != NULL){//Explorar en la dimension actual
            if((*otherCurrentNode)->nextTrieNode != NULL)
                compareTrie(&(*currentNode)->nextTrieNode,&(*otherCurrentNode)->nextTrieNode,compare);
            else{
                (*compare) = false;
                return;
            }
        }else
            if((*otherCurrentNode)->nextTrieNode != NULL){
                (*compare) = false;
                return;
            }
        
        if((*compare) == false)
            return;
    }
    (*compare) = true;
}

/*Método Auxiliar para clonar un EVM.
 * Argumentos:
 * -Nodo raíz del arbol original.
*/
/**
 * Método para clonar un EVM.
 * @return nDEVM *, apuntador al nuevo EVM.
 */
nDEVM * nDEVM::cloneEVM(){
    trieNode *copyRootNode = NULL;
    trieNode *prevNode = NULL;
    trieNode *copyPrevNode = NULL;
    //double * testKey = new double[4];
    cloneTrie(&prevNode,&rootNode,&copyPrevNode,&copyRootNode);
    nDEVM *cloneEVM = new nDEVM(copyRootNode);
    return cloneEVM;
}


/*Método Auxiliar para clonar un árbol trie.
 * Argumentos:
 * -Nodo raíz del arbol original.
*/
trieNode * nDEVM::cloneTrie(){
    trieNode *copyRootNode = NULL;
    trieNode *prevNode = NULL;
    trieNode *copyPrevNode = NULL;
    //double * testKey = new double[4];
    cloneTrie(&prevNode,&rootNode,&copyPrevNode,&copyRootNode);
    return copyRootNode;
}

/**
 * Método Principal para clonar un árbol trie.
 * @param prevNode: Nodo previo, del Trie original, en la exploración.
 * @param currentNode: Nodo actual, del Trie original, en la exploración.
 * @param copyPrevNode: Nodo previo, de la copia, en la exploración.
 * @param copyCurrentNode: Nodo actual, de la copia, en la exploración.
 */
void nDEVM::cloneTrie(trieNode **prevNode,trieNode **currentNode,trieNode **copyPrevNode,trieNode **copyCurrentNode){
    if((*currentNode) == NULL){//Si el arbol esta vacio
        return;
    }else{
        trieNode *node = new trieNode;
        node->nextDim = NULL;
        node->nextTrieNode = NULL;        
        node->value = (*currentNode)->value;
        (*copyCurrentNode) = node;
        
        if((*prevNode) != NULL)//Si no es le nodo raiz
            if((*prevNode)->nextDim == (*currentNode)){//si se llega por profundidad
                (*copyPrevNode)->nextDim = (*copyCurrentNode);
            }else{//Se llega por amplitud
                (*copyPrevNode)->nextTrieNode = (*copyCurrentNode);
            }
            
        if((*currentNode)->nextDim != NULL)//Si no es nodo hoja, explorar la siguiente dimension
            cloneTrie(currentNode,&(*currentNode)->nextDim,copyCurrentNode,&(*copyCurrentNode)->nextDim);

        if((*currentNode)->nextTrieNode != NULL){//Explorar en la dimension actual
            cloneTrie(currentNode,&(*currentNode)->nextTrieNode,copyCurrentNode,&(*copyCurrentNode)->nextTrieNode);
        }
    }
}

/*Método Auxiliar para remover un vertice de un árbol trie.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Apuntador al vector del elemento a eliminar.
*/
void nDEVM::removeVertex(double *key){
    trieNode * prevNode = NULL;
    removeVertex(&prevNode,&rootNode,key,0);
}

/*Método Principal Recursivo para remover un vertice de un árbol trie.
 * Argumentos:
 * -Nodo previo.
 * -Nodo raíz del arbol.
 * -Apuntador al vector del elemento a eliminar.
*/
/**
 * Método principal para remover un vértice de un árbol Trie.
 * @param prevNode: Nodo previo en la exploración.
 * @param currentNode: Nodo actual en la exploración.
 * @param key: Arreglo que contiene el vértice que se desea eliminar.
 * @param currentDim: Dimensión actual en la exploración.
 * @return Un valor booleano, que indica si se puede intentar eliminar el nodo actual,
 * ya que probablemente tenga relación con otros nodos.
 */
bool nDEVM::removeVertex(trieNode **prevNode,trieNode **currentNode,double *key,int currentDim){
    //Si se llega al un nodo hoja, entonces si existe el vertice
    //Si el arbol esta vacio desde el inicio, no habra llamada recursiva que haga algo con este valor TRUE
    if((*currentNode) == NULL)
        return true;

    //Si hay un nodo con un valor mayor en la dimension actual, entonces no existe el nodo
    if((*currentNode)->value > key[currentDim])
        return false;

    //Si el nodo actual tiene un valor igual en la dimension actual
    if((*currentNode)->value == key[currentDim])
    {
        //Si el valor devuelto por la llamada recursiva es true, entonces se puede intentar eliminar el nodo actual
        bool vertexStatus = removeVertex(currentNode,&(*currentNode)->nextDim,key,currentDim+1);
        if(vertexStatus){
            //Si es el nodo raiz
            if(rootNode == (*currentNode)){
                //Si existe un nodo siguiente en la primer dimension
                if(rootNode->nextTrieNode != NULL){
                    trieNode *tempNode = rootNode;
                    rootNode = rootNode->nextTrieNode;
                    delete tempNode;
                    tempNode = NULL;
                }else{
                    rootNode == NULL;
                    delete *currentNode;
                    *currentNode = NULL;
                }
                
                return false;
            }else{

                //Si se llega por profundidad
                if((*prevNode)->nextDim == (*currentNode))
                {
                    //Si no hay mas nodos a la derecha
                    if((*currentNode)->nextTrieNode == NULL)
                    {
                        delete *currentNode;
                        *currentNode = NULL;
                        //Intentar eliminar nodo padre
                        return true;
                    }else
                    {
                        trieNode *node = (*currentNode)->nextTrieNode;
                        delete *currentNode;
                        *currentNode = NULL;
                        (*prevNode)->nextDim = node;
                        //Nodo padre no puede ser eliminado
                        return false;
                    }
                }else
                {
                    //Se llega por amplitud, busqueda en la misma dimension
                    if((*currentNode)->nextTrieNode == NULL)
                    {
                        delete *currentNode;
                        *currentNode = NULL;
                    }else
                    {
                        trieNode *node = (*currentNode)->nextTrieNode;
                        delete *currentNode;
                        *currentNode = NULL;
                        (*prevNode)->nextTrieNode= node;
                    }
                    //Nodo padre no puede ser eliminado
                    return false;
                }                
            }
        }
    }

    if((*currentNode)->nextTrieNode != NULL)//Explorar en la dimension actual
        return removeVertex(currentNode,&(*currentNode)->nextTrieNode,key,currentDim);
    
    return false;//No se hallo el valor para la dimension actual
}

/*Método Auxiliar para verificar que un vertice exista en un árbol trie.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Apuntador al vector del vertice.
 * -Tamaño del vector.
*/
bool nDEVM::existsVertex(double * inputKey,int length){
    return existsVertex(&rootNode,inputKey,length,0);
}

/*Método Principal Recursivo para verificar que un vertice exista en un árbol trie.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Apuntador al vector del vertice.
 * -Tamaño del vector.
 * -Dimension Actual.
*/
bool nDEVM::existsVertex(trieNode **currentNode,double * inputKey,int length,int currentDim){
    //Si se llega a explorar hasta el punto donde la dimension actual no es menor que el tamaño del arreglo (0,1,...,n),
    //entonces el vertice si existe
    if(!(currentDim < length))
        return true;   //El vertice si existe
    
    if(*currentNode == NULL){
        return false;    //El vertice no existe
    }else{
        //Si el nodo actual tiene un valor igual en la dimension actual, explorar siguiente dimension
        if((*currentNode)->value == inputKey[currentDim]){
            return existsVertex(&((*currentNode)->nextDim),inputKey,length, currentDim+1);
        }else    
            //Si eñ nodo actual tiene un valor mayor en la dimension actual, entonces no existe
            if((*currentNode)->value > inputKey[currentDim]){
                return false;
            }else   //Se explora otro nodo en la dimension actual
                return existsVertex(&((*currentNode)->nextTrieNode),inputKey,length, currentDim);
    }
}

/*Método Auxiliar para obtener un sub-árbol trie de otro, usando un valor como referencia.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Valor.
*/
trieNode * nDEVM::getSubTrie(double key){
    return getSubTrie(&rootNode,key);
}

/*Método para obtener un sub-árbol trie de otro, usando un valor como referencia.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Valor.
*/
trieNode * nDEVM::getSubTrie(trieNode **currentNode,double key){
    if((*currentNode) == NULL)
        return NULL;
    
    if((*currentNode)->value == key)
        return (*currentNode)->nextDim;
    
    if((*currentNode)->value > key)//Si hay un valor mayor a la llave, entonces no existe el subarbol
        return NULL;

    if((*currentNode)->nextTrieNode != NULL)//Explorar en la dimension actual
        return getSubTrie(&(*currentNode)->nextTrieNode,key);
    else
        return NULL;
}

/*Método Auxiliar para obtener un sub-árbol trie de otro, usando un contador como referencia.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Valor del contador.
*/
nDEVM* nDEVM::couplet(double key){
    nDEVM *couplet = new nDEVM(getSubTrie2(&rootNode,key));
    return couplet;
}

/*Método Auxiliar para obtener un sub-árbol trie de otro, usando un contador como referencia.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Valor del contador.
*/
trieNode * nDEVM::getSubTrie2(double key){
    return getSubTrie2(&rootNode,key);
}

/*Método para obtener un sub-árbol trie de otro, usando un contador como referencia.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Valor del contador.
*/
trieNode * nDEVM::getSubTrie2(trieNode **currentNode,double key){
    if((*currentNode) == NULL)
        return NULL;
    
    if(key == 0)
        return (*currentNode)->nextDim;

    if((*currentNode)->nextTrieNode != NULL)//Explorar en la dimension actual
        return getSubTrie2(&(*currentNode)->nextTrieNode,key-1);
    else
        return NULL;   
}

/*Método para realizar la operación XOR de dos EVMs.
 * Argumentos:
 * -Apuntador al otro EVM.
 * -Dimension
*/
nDEVM* nDEVM::mergeXOR(nDEVM* otherEVM){
    //Se considera que esta operacion se realizara con EVMs de la misma dimension.
    if(rootNode == NULL and otherEVM->rootNode != NULL)
        return otherEVM->cloneEVM();
   
    if(rootNode != NULL and otherEVM->rootNode == NULL)
        return cloneEVM();

    if(rootNode == NULL and otherEVM->rootNode == NULL){
        nDEVM *xorEVM = new nDEVM();
        return xorEVM;
    }
        

    int dimDepth = getDimDepth();
    //Se clona el trie del EVM desde donde se llama la operacion mergeXOR.
    nDEVM *xorEVM = new nDEVM(cloneTrie());
    
    double *key = new double[dimDepth];  //ELIMINAR
    //Hay problemas con el apuntador a root...
    //nDEVM *xorEVM = new nDEVM(XORTrie(&(otherEVM->rootNode),dimDepth));
    
    //Para no perder referencia al root desde la funcion insertVertex...
    xorEVM->XORTrie(&(xorEVM->rootNode),&(otherEVM->rootNode),&key,0);
    return xorEVM;
}

/*Método para realizar la operación XOR de dos tries.
 * Argumentos:
 * -Nodo raíz del primer arbol trie.
 * -Nodo raíz del segundo arbol trie.
 * -Dimensión.
*/
trieNode * nDEVM::XORTrie(trieNode **otherTrie,int dim){
    trieNode *resultTrie = NULL;
    //Se clona el primer trie.
    resultTrie = cloneTrie();
    double *key = new double[dim];  //ELIMINAR
    //Se realiza la inserción de los elementos del segundo trie en el clon del primero.
    XORTrie(&resultTrie,otherTrie,&key,0);
    return resultTrie;
}

/*Método Principal Recursivo para realizar la operación XOR de dos tries.
 * Argumentos:
 * -Nodo raíz del primer arbol trie.
 * -Nodo raíz del segundo arbol trie.
 * -Apuntador doble del vector para almacenar los elementos.
 * -Dimensión.
*/
void nDEVM::XORTrie(trieNode **resultTrie,trieNode **currentNode,double **key,int dim){
    if((*currentNode) == NULL){//Si el arbol esta vacio
        //cout<<vectorToString(key,dim)<<'\n';
        insertVertex(resultTrie,(*key),dim);
        return;
    }

    (*key)[dim] = (*currentNode)->value;

    XORTrie(resultTrie,&(*currentNode)->nextDim,key,dim+1);

    if((*currentNode)->nextTrieNode != NULL)//Explorar en la dimension actual
        XORTrie(resultTrie,&(*currentNode)->nextTrieNode,key,dim);
}

/*Método para vaciar un archivo .raw a un trie.
 * Argumentos:
 * -Nombre del archivo.
 * -Dimensiones.
*/
void nDEVM::rawFileToEVM(string fileName,int x1,int x2,int x3){
    //const char * file = "VL-vismale-(128x256x256)-(1.5,1,1).raw";
    ifstream fileInput;
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file

    unsigned char buffer;
    double *newKey = new double[4]; //ELIMINAR
    double value;
    
    if (fileInput.is_open())
    {
        cout << "Se abrio correctamente el archivo: "<< fileName << "\n";
        
        //Primeramente, se hace un barrido en la dimensión 3
        for(int i = 0; i < x3; i++){
            newKey[3] = i;
            //Se hace el barrido en la dimensión 2
            for(int j = 0; j < x2; j++){
                newKey[2] = j;
                //Se hace el barrido en la dimensión 1
                for(int w = 0; w < x1; w++)
                {                    
                    newKey[1] = w;
                    //Se lee 1 Byte de información a la vez
                    if(fileInput.read((char *)( &buffer ), sizeof(buffer)))
                    {
                        value = buffer;
                        newKey[0] = 0.0;
                        populate3DVoxel(&newKey);   //Corregir: Insertar en un metodo populate4DVoxel
                        newKey[0] = value+1.0;
                        populate3DVoxel(&newKey);
                    }
                }
            }
        }

        fileInput.close();
    }else
        cout << "No se abrio correctamente el archivo: "<< fileName << "\n";
}

/**
 * Cargar un archivo binario, que representa una voxelizacion en 3D, en el EVM...
 * @param fileName
 * @param voxelSize
 */
void nDEVM::loadnDRawFile(string fileName,int voxelSize,int dim){
    ifstream inputFile;
    inputFile.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file

    
    double *newVoxel = new double[dim]; //ELIMINAR
    
    if(!inputFile.is_open()){
        cout << "No se pudo abrir el archivo: "<< fileName << "\n";
        return;
    }
    
    voxelizeRawFile(&newVoxel,&inputFile,voxelSize,dim,dim);
    
    inputFile.close();
}

void nDEVM::voxelizeRawFile(double **voxelInput,ifstream *inputFile,int voxelSize,int dim, int currentDim){
    if(currentDim == 0){
        unsigned char buffer;
        //Se lee 1 Byte de información a la vez
        if((*inputFile).read((char *)( &buffer ), sizeof(buffer)))
        {
            // Si el voxel esta lleno, se carga en el EVM
            if(buffer == 1){
                populateVoxel(voxelInput,dim,0,0);
            }
        }
        return;
    }

    for(int i = 0; i < voxelSize; i++){
        (*voxelInput)[currentDim - 1] = i;
        voxelizeRawFile(voxelInput,inputFile,voxelSize,dim,currentDim-1);
    }
}

/**
 * Cargar un archivo binario, que representa una voxelizacion en 3D, en el EVM...
 * @param fileName
 * @param voxelSize
 */
void nDEVM::load3DRawFile(string fileName,int voxelSize){
    ifstream fileInput;
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file

    unsigned char buffer;
    double *newVoxel = new double[3]; //ELIMINAR
    
    if(!fileInput.is_open()){
        cout << "No se pudo abrir el archivo: "<< fileName << "\n";
        return;
    }
    
    //Primeramente, se hace un barrido en la dimensión 3
    for(int x3 = 0; x3 < voxelSize; x3++){
        newVoxel[2] = x3;
        //Se hace el barrido en la dimensión 2
        for(int x2 = 0; x2 < voxelSize; x2++){
            newVoxel[1] = x2;
            //Se hace el barrido en la dimensión 1
            for(int x1 = 0; x1 < voxelSize; x1++)
            {
                newVoxel[0] = x1;
                //Se lee 1 Byte de información a la vez
                if(fileInput.read((char *)( &buffer ), sizeof(buffer)))
                {
                    //cout<< "Buffer: "<< buffer;
                    // Si el voxel esta lleno, se carga en el EVM
                    if(buffer == 1){
                        populateVoxel(&newVoxel,3,0,0);
                    }
                }
            }
        }
    }

    fileInput.close();
}

/**
 * Cargar un archivo binario, que representa una voxelizacion en 3D, en el EVM...
 * @param fileName
 * @param voxelSize
 */
void nDEVM::load2DRawFile(string fileName,int voxelSize){
    ifstream fileInput;
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file

    unsigned char buffer;
    double *newVoxel = new double[2]; //ELIMINAR
    
    if(!fileInput.is_open()){
        cout << "No se pudo abrir el archivo: "<< fileName << "\n";
        return;
    }
    
    //Se hace el barrido en la dimensión 2
    for(int x2 = 0; x2 < voxelSize; x2++){
        newVoxel[1] = x2;
        //Se hace el barrido en la dimensión 1
        for(int x1 = 0; x1 < voxelSize; x1++)
        {
            newVoxel[0] = x1;
            //Se lee 1 Byte de información a la vez
            if(fileInput.read((char *)( &buffer ), sizeof(buffer)))
            {
                //cout<< "Buffer: "<< buffer;
                // Si el voxel esta lleno, se carga en el EVM
                if(buffer == 1){
                    populateVoxel(&newVoxel,2,0,0);
                }
            }
        }
    }

    fileInput.close();
}

/*Método para generar e insertar la voxelización que consiste de 8 vértices, generada a partir del vértice en el origen.
 * Argumentos:
 * -Nodo Raiz.
 * -Apuntador doble del vector de entrada.
*/
void nDEVM::populate3DVoxel(double **voxelInput){
    populateVoxel(voxelInput,3,0,1);
}

void nDEVM::populate2DVoxel(double **voxelInput){
    populateVoxel(voxelInput,2,0,0);
}

/*Método Principal y Recursivo para generar e insertar la voxelización que consiste de 8 vértices, generada a partir del vértice en el origen.
 * Argumentos:
 * -Nodo Raiz.
 * -Apuntador doble del vector de entrada.
*/
void nDEVM::populateVoxel(double **voxelInput,int dim,int currentDim,int offset){
    if(!(currentDim < dim)){
        insertVertex(*voxelInput,dim+offset);
        //cout<<vectorToString(&voxelInput,dim+1)<<endl;
        return;
    }
    populateVoxel(voxelInput,dim,currentDim+1,offset);
    (*voxelInput)[currentDim+offset] = (*voxelInput)[currentDim+offset]+1;
    populateVoxel(voxelInput,dim,currentDim+1,offset);
    (*voxelInput)[currentDim+offset] = (*voxelInput)[currentDim+offset]-1;
}

/*Método para vaciar un arbol trie en un archivo de texto .evm
 * Argumentos:
 * -Nodo Raiz.
*/
void nDEVM::EVMFile(int index){
    double * testKey = new double[4];   //Eliminar
    string fileName = "EVMFiles/EVMFile_"+to_string(index)+".evm";
    
    ofstream outputFile( fileName );
    if ( ! outputFile.is_open() ){    
        cout << "El archivo no se pudo abrir!" << '\n';    
        return;
    } 
    outputFile<<"XYZ"<<'\n'<<'3'<<'\n';
    EVMFile(&outputFile,rootNode,&testKey,0);
    outputFile.close();
}

/*Método Principal y Recursivo para vaciar un arbol trie en un archivo de texto .evm
 * Argumentos:
 * -APuntador al archivo en donde se almacena la información.
 * -Nodo Raiz.
 * -Apuntador doble al vector que se usa para obtener la información
 * -Dimension.
*/
void nDEVM::EVMFile(ofstream *outputFile,trieNode *currentNode,double **key, int dim){
    if((currentNode) == NULL){   //que el arbol no este vacio
        *outputFile << vectorToString2(key,dim)<<'\n';
        return;
    }

    (*key)[dim] = (currentNode)->value;
    EVMFile(outputFile,(currentNode)->nextDim, key, dim+1);

    if((currentNode)->nextTrieNode != NULL){
        EVMFile(outputFile,(currentNode)->nextTrieNode, key, dim);
    }
}

/*Método para Imprimir en consola un vector.
 * Argumentos:
 * -Apuntador doble del vector.
 * -Tamaño del vector.
*/
string nDEVM::vectorToString(double **vector,int size){
    string output="(  ";
    for(int i =0;i<size;i++)
        output+=to_string((*vector)[i])+"  ";
    return output+")";
}

/*Método que retorna en texto el contenido de un vector
 * Argumentos:
 * -Apuntador doble al vector
 * -Tamaño del vector
*/
string nDEVM::vectorToString2(double **vector,int size){
    string output="";
    char s[5];
    for(int i =0;i<size;i++){
        if(i != 0)
            output+=' ';
        sprintf(s,"%.1f", (*vector)[i]);
        output+=to_string(s);
    }
    return output;
}

/*
 *Metodo auxiliar para agregar un Couplet perpendicular a la dimension x1:
 * Argumentos:
 * -El (n-1)D EVM del couplet.
 */
void nDEVM::putCouplet(nDEVM * couplet){
    trieNode * prevNode = NULL;
    //CLONE???????
    //trieNode* coupletTrie = couplet->cloneTrie();//Clonar el trie del Couplet
    putCouplet(&prevNode,&rootNode,couplet->rootNode);
}

/*
 *Metodo Principal para agregar un Couplet perpendicular a la dimension x1 en la posicion dada:
 * Argumentos:
 * -El EVM del couplet.
 */
void nDEVM::putCouplet(trieNode** prevNode,trieNode **currentNode,trieNode *coupletRoot){
    if((*currentNode) == NULL){
        if((*prevNode) != NULL)//AL menos hay un nodo en el Trie
            (*prevNode)->nextTrieNode = coupletRoot;
        else//El trie esta vacio
            *currentNode = coupletRoot;
            
        return;
    }
    
    putCouplet(currentNode,&((*currentNode)->nextTrieNode),coupletRoot);
}

/*
 *Metodo auxiliar para agregar una Seccion perpendicular a la dimension x1:
 * Argumentos:
 * -El (n-1)D EVM del couplet.
 */
void nDEVM::putSection(nDEVM * section){
    trieNode * prevNode = NULL;
    //CLONE???????
    //trieNode* coupletTrie = couplet->cloneTrie();//Clonar el trie del Couplet
    putCouplet(&prevNode,&rootNode,section->rootNode);
}

/**
 * Obtiene el couplet de la primera dimension asociado a la posicion donde
 * actualmente se encuentra el apuntador de couplets coupletIndex.
 * @return 
 * nDEVM*
 */
nDEVM* nDEVM::readCouplet(){
    if(!endEVM()){
        nDEVM* coupletEVM = new nDEVM((*coupletIndex)->nextDim);
        coupletIndex = &((*coupletIndex)->nextTrieNode);
        return coupletEVM;
    }else
        return NULL;
}

/**
 * Obtiene el couplet de la primera dimension asociado a la posicion donde
 * actualmente se encuentra el apuntador de couplets coupletIndex.
 * @return 
 * nDEVM*
 */
nDEVM* nDEVM::readSection(){
    return readCouplet();
}

/**
 * Metodo que indica si se ha alcanzado el ultimo couplet, en base a la posicion
 * del indice de couplets.
 * @return 
 * Boolean
 */
bool nDEVM::endEVM(){
    if(*coupletIndex == NULL)
        return true;
    else
        return false;
}

/**
 * Metodo que inserta un nuevo nodo en la dimension x1 con el valor dado, y 
 * el nuevo nodo sera el nodo raiz.
 * @param coord
 * @return 
 */
void nDEVM::setCoord(double coord){
    //CLONE?????
    //trieNode* coupletTrie = cloneTrie();
    trieNode* coupletRoot = new trieNode;
    coupletRoot->value = coord;
    coupletRoot->nextTrieNode = NULL;
    coupletRoot->nextDim = rootNode;
    rootNode = coupletRoot;
}

/**
 * Metodo que devuelve el valor de la primera dimension del couplet asociado al
 * nodo raiz.
 * @return 
 */
double nDEVM::getCoord(){
    return rootNode->value;
}

nDEVM* nDEVM::getSection(nDEVM* section, nDEVM *couplet){
    return section->mergeXOR(couplet);
}

nDEVM* nDEVM::getCouplet(nDEVM* section1, nDEVM *section2){
    return section1->mergeXOR(section2);
}

nDEVM* nDEVM::EVMSectionSequence(){
    nDEVM* sectionSequence = new nDEVM();
    EVMSectionSequence(&sectionSequence);
    return sectionSequence;
}

void nDEVM::EVMSectionSequence(nDEVM** sectionSequence){
    nDEVM* currentCouplet = new nDEVM();
    nDEVM* prevSection = new nDEVM();
    nDEVM* currentSection = new nDEVM();
    currentCouplet = readCouplet();
    int i = 0;
    while(!endEVM()){
        currentSection = getSection(prevSection,currentCouplet);
//        cout<<"Section "<<i<<": "<<endl;
//        currentSection->printTrie();
        
        // Procesamiento
        if(i > 0){
            prevSection->setCoord(i - 1);
            (*sectionSequence)->putSection(prevSection);
        }
        
        //currentSection->EVMFile(i);
        //cout<<endl;
        
        // Siguiente Iteración
        prevSection = currentSection;
        currentCouplet = readCouplet();
        i++;
    }
    currentSection->setCoord(i - 1);
    (*sectionSequence)->putSection(currentSection);
    resetCoupletIndex();
}

nDEVM* nDEVM::EVMCoupletSequence(){
    nDEVM* coupletSequence = new nDEVM();
    EVMCoupletSequence(&coupletSequence);
    return coupletSequence;
}

void nDEVM::EVMCoupletSequence(nDEVM** coupletSequence){
    nDEVM* currentCouplet = new nDEVM();
    nDEVM* prevSection = new nDEVM();
    nDEVM* currentSection = readSection();
    int i = 0;
    while(true){
        currentCouplet = getSection(prevSection,currentSection);
        //cout<<"Couplet "<<i<<": "<<endl;
        //currentCouplet->printTrie();
        
        // Procesamiento
        currentCouplet->setCoord(i);
        (*coupletSequence)->putCouplet(currentCouplet);

        //currentSection->EVMFile(i);
        //cout<<endl;
        
        if(endEVM()){
            currentCouplet = currentSection->cloneEVM();
            currentCouplet->setCoord(i+1);
            (*coupletSequence)->putCouplet(currentCouplet);
            break;
        }
        // Siguiente Iteración
        prevSection = currentSection;
        currentSection = readSection();
        i++;
    }
    resetCoupletIndex();
}
/**
 * Metodo que devuelve la profundidad dimensional de un trie.
 * @return 
 */
int nDEVM::getDimDepth(){
    return getDimDepth(rootNode,0);
}

/**
 * Metodo que devuelve la profundidad dimensional de un trie.
 * @param currentNode
 * @param dim
 * @return 
 */
int nDEVM::getDimDepth(trieNode* currentNode,int dim){
    if(currentNode == NULL)
        return dim;
    
    return getDimDepth(currentNode->nextDim,dim+1);
}

/**
 * Método para realizar una operación booleana entre dos EVMs.
 * @param evm1
 * @param evm2
 * @param op
 * @return 
 */
nDEVM* nDEVM::booleanOperation(nDEVM* evm2, string op, int n){
    return booleanOperation(this,evm2,op,n);
}

nDEVM* nDEVM::booleanOperation(nDEVM *p, nDEVM *q, string op, int n){
    nDEVM *pSection, *qSection, *couplet;
    nDEVM *result, *rPrevSection, *rCurrentSection;
    bool fromP, fromQ;
    double coord;
    if(n == 1){
        return unionOperation(p,q);
    }
    pSection = new nDEVM();
    qSection = new nDEVM();
    rCurrentSection = new nDEVM();
    result = new nDEVM();
    while(!(p->endEVM()) and !(q->endEVM())){
        nextObject(p,q,&coord,&fromP,&fromQ);
        if(fromP){
            couplet = p->readCouplet();
//            cout<<"section fromP, coord: "<<coord<<"\n";
            pSection = getSection(pSection,couplet);
//            pSection->printTrie();
        }
        if(fromQ){
            couplet = q->readCouplet();
//            cout<<"section fromQ, coord: "<<coord<<"\n";
            qSection = getSection(qSection,couplet);
//            qSection->printTrie();
        }
        rPrevSection = rCurrentSection;
        rCurrentSection = booleanOperation(pSection, qSection,op,n-1);
        
//        cout<<"\nSection Result: \n";
//        rCurrentSection->printTrie();
        
        couplet = getCouplet(rPrevSection,rCurrentSection);

//        cout<<"\nCouplet Result: \n";
//        couplet->printTrie();
//        cout<<"---\n";

        if(!couplet->isEmpty()){
            couplet->setCoord(coord);

            result->putCouplet(couplet);
        }
    }
    while(!(p->endEVM())){
        if(putCoupletByOp(op,1)){
            coord = (*(p->coupletIndex))->value;
            couplet = (p->readCouplet())->cloneEVM();
            couplet->setCoord(coord);
            result->putCouplet(couplet);
        }else
            break;
    }

    while(!(q->endEVM())){
        if(putCoupletByOp(op,2)){
            coord = (*(q->coupletIndex))->value;
            couplet = (q->readCouplet())->cloneEVM();
            couplet->setCoord(coord);
            result->putCouplet(couplet);
        }else
            break;
    }
    p->resetCoupletIndex();
    q->resetCoupletIndex();
    return result;
}

void nDEVM::nextObject(nDEVM *p, nDEVM *q,double *coord,bool *fromP, bool *fromQ){
    if((*(p->coupletIndex))->value < (*(q->coupletIndex))->value){
        *coord = (*(p->coupletIndex))->value;
        *fromP = true;
        *fromQ = false;
        return;
    }

    if((*(q->coupletIndex))->value < (*(p->coupletIndex))->value){
        *coord = (*(q->coupletIndex))->value;
        *fromQ = true;
        *fromP = false;
        return;
    }

    if((*(p->coupletIndex))->value == (*(q->coupletIndex))->value){
        *coord = (*(p->coupletIndex))->value;
        *fromQ = true;
        *fromP = true;
        return;
    }
}

nDEVM* nDEVM::unionOperation(nDEVM* section1, nDEVM* section2){
    // Casos en el que uno o ambos EVMs estan vacios
    if(section1->isEmpty() and !section2->isEmpty()){
        return section2->cloneEVM();
    }
    
    if(section2->isEmpty() and !section1->isEmpty()){
        return section1->cloneEVM();
    }
    
    if(section1->isEmpty() and section2->isEmpty()){
        return new nDEVM();
    }
    
    // Variante de la solucion para la operacion union
    nDEVM *result = new nDEVM();
    trieNode *subSection1 = section1->getRootNode(), *subSection2 = section2->getRootNode();
    trieNode **prevResult, **currentResult = &result->rootNode;

    // Para el caso de la UNION se considera a result vacio inicialmente   
    // Mientras ninguno de los dos llegue al final
    while(subSection1 != NULL and subSection2 != NULL){
        if(subSection1->value <= subSection2->value){
            unionOperation(*currentResult,subSection1,&result);
            //Pasar al siguiente segmento
            subSection1 = subSection1->nextTrieNode->nextTrieNode;
        }else{
            unionOperation(*currentResult,subSection2,&result);
            //Pasar al siguiente segmento
            subSection2 = subSection2->nextTrieNode->nextTrieNode;
        }
        
        if((*currentResult)->nextTrieNode->nextTrieNode != NULL){
            prevResult = &((*currentResult)->nextTrieNode->nextTrieNode);
            currentResult = &((*currentResult)->nextTrieNode->nextTrieNode);
        }
    }
   
    if(subSection1 != NULL){
        while(subSection1 != NULL){
            //Se realiza la union con el ultimo segmento del trie resultante
            unionOperation(*currentResult,subSection1,&result);
            subSection1 = subSection1->nextTrieNode->nextTrieNode;

            if((*currentResult)->nextTrieNode->nextTrieNode != NULL){
                prevResult = &((*currentResult)->nextTrieNode->nextTrieNode);
                currentResult = &((*currentResult)->nextTrieNode->nextTrieNode);
            }
        }
    }
    
    if(subSection2 != NULL){
        while(subSection2 != NULL){
            //Se realiza la union con el ultimo segmento del trie resultante
            unionOperation(*currentResult,subSection2,&result);
            subSection2 = subSection2->nextTrieNode->nextTrieNode;

            if((*currentResult)->nextTrieNode->nextTrieNode != NULL){
                prevResult = &((*currentResult)->nextTrieNode->nextTrieNode);
                currentResult = &((*currentResult)->nextTrieNode->nextTrieNode);
            }
        }
    }
    
//    cout<<"UnionTrieResult: \n";
//    result->printTrie();
    
    return result;
}

/**
 * Metodo que realiza la operacion Union entre dos secciones 1D, este es el caso base. 
 * Las secciones 1D de entrada consisten en un par de vértices, que acotan una línea.
 * @param section1: apuntador al nodo inicial de la primer seccion 1D, el nodo 
 * final se conoce como section1->nextTrieNode.
 * @param section2: apuntador al nodo inicial de la segunda seccion 1D, el nodo 
 * final se conoce  como section2->nextTrieNode.
 * @return 
 */
void nDEVM::unionOperation(trieNode* section1, trieNode* section2,nDEVM **result){
    // Despues de la primera iteracion, el primer argumento correpondera a un segmento del EVM resultante
    // Los vertices estan ordenados de menor a mayor.
    double vertex[1];
    double a,b,c,d;
    
    // Validaciones para el caso de que uno o ambos sean nulos
    if(section1 == NULL and section2 == NULL){
//        cout<<"Caso 0.1: Ambos Nulos...\n";
        return;
    }
    
    if(section1 != NULL and section2 == NULL){
//        cout<<"Caso 0.2: Section1 Nulo...\n";
        vertex[0] = section1->value;
        (*result)->insertVertex(vertex,1);
        vertex[0] = section1->nextTrieNode->value;
        (*result)->insertVertex(vertex,1);
        return;
    }
        
    if(section1 == NULL and section2 != NULL){
//        cout<<"Caso 0.3: Section2 Nulo...\n";
        vertex[0] = section2->value;
        (*result)->insertVertex(vertex,1);
        vertex[0] = section2->nextTrieNode->value;
        (*result)->insertVertex(vertex,1);
        return;
    }

    // Para una situacion normal
    // Tomando a--b del EVM resultante
    a = section1->value, b = section1->nextTrieNode->value; // Arista a---b
    c = section2->value, d = section2->nextTrieNode->value; // Arista c---d
    
    // Caso 1: las aristas son disjuntas
    // Aparentemente solo se presentara el caso: a--b c--d
    if(b < c or d < a){
//        cout<<"Caso 1: las aristas son disjuntas\n";
        vertex[0] = a;
        (*result)->condInsertVertex(vertex,1);
        vertex[0] = b;
        (*result)->condInsertVertex(vertex,1);
        vertex[0] = c;
        (*result)->condInsertVertex(vertex,1);
        vertex[0] = d;
        (*result)->condInsertVertex(vertex,1);
        return;
    }
    
    // Caso 2.1: las aristas son contiguas;  Caso 5.1: Superposicion a--(b,c)--d
    if(a < c and b >= c and b < d){
//        cout<<"Caso 2.1: las aristas son contiguas;  Caso 5.1: Superposicion a--(b,c)--d\n";
        // Eliminar si no es la primera iteracion
        vertex[0] = b;
        (*result)->removeVertex(vertex);
        
        vertex[0] = a;
        (*result)->condInsertVertex(vertex,1);

        vertex[0] = d;
        (*result)->condInsertVertex(vertex,1);
        return;
    }
    
    // Caso 2.2: las aristas son contiguas;  Caso 5.2: Superposicion c--(d,a)--b
    if(c < a and d >= a and d < b){
        // Aparentemente no pasara
        // Eliminar si no es la primera iteracion
//        cout<<"Caso 2.2: las aristas son contiguas;  Caso 5.2: Superposicion c--(d,a)--b\n";
        vertex[0] = a;
        (*result)->removeVertex(vertex);
        
        vertex[0] = c;
        (*result)->condInsertVertex(vertex,1);
        vertex[0] = b;
        (*result)->condInsertVertex(vertex,1);
        return;
    }
       
    // Caso 3: Las aristas coinciden
    // Caso 4.1: Inclusion
    // * La arista 2 esta adentro de la arista 1.
    if(a <= c and d <= b){
//        cout<<"Caso 3: Las aristas coinciden;  Caso 4.1: Inclusion\n";
        vertex[0] = a;
        (*result)->condInsertVertex(vertex,1);
        vertex[0] = b;
        (*result)->condInsertVertex(vertex,1);
        return;
    }

    // Caso 4.2: Inlusion en el otro sentido
    if(c <= a and b <= d){
        // Aparentemente no pasara
//        cout<<"Caso 4.2: Inlusion en el otro sentido\n";
        vertex[0] = a;
        (*result)->removeVertex(vertex);
        vertex[0] = b;
        (*result)->removeVertex(vertex);

        vertex[0] = c;
        (*result)->condInsertVertex(vertex,1);
        vertex[0] = d;
        (*result)->condInsertVertex(vertex,1);
        return;
    }
}

bool nDEVM::putCoupletByOp(string op,int argPosition){
    if(op.compare("union") == 0){
        return true;
    }
    
    return false;
}

void nDEVM::condInsertVertex(double * inputKey,int length){
    if(!existsVertex(inputKey,length))
        insertVertex(inputKey, length);
}
