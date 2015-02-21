/* 
 * File:   trieTree.cpp
 * Author: yova
 * 
 * Created on 24 de octubre de 2014, 12:57 PM
 */
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <bitset>

#include "TrieTree.h"

#include "BMP.h"

template <typename T> std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

TrieTree::TrieTree() {
    rootNode = NULL;
    resetCoupletIndex();
}

TrieTree::TrieTree(trieNode *node) {
    rootNode = node;
    resetCoupletIndex();
}

TrieTree::TrieTree(const TrieTree& orig) {
}

TrieTree::~TrieTree() {
    deleteTrie();    
}

void TrieTree::deleteTrie(){
    deleteTrie(rootNode);    
}

void TrieTree::deleteTrie(trieNode *currentNode){
    if((currentNode) == NULL){
        return;
    }

    deleteTrie((currentNode)->nextDim);

    if((currentNode)->nextTrieNode != NULL){
        deleteTrie((currentNode)->nextTrieNode);
    }

    delete currentNode;
    currentNode = NULL;
}

void TrieTree::resetCoupletIndex(){
    coupletIndex = &rootNode;
}

bool TrieTree::isEmpty(){
    if(rootNode == NULL)
        return true;
    else
        return false;
}

bool TrieTree::endTrie(){
    if(*coupletIndex == NULL)
        return true;
    else
        return false;
}

void TrieTree::insertVertex(double * inputKey,int length){
    trieNode * prevNode = NULL;
    insertVertex(&prevNode,&rootNode,inputKey,length,0,0,length);
}

bool TrieTree::insertVertex(trieNode **prevNode,trieNode **currentNode,double * inputKey,
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

bool TrieTree::compare(TrieTree *otherTrie){
    bool compare1 = true, compare2 = true;
    compare(&rootNode,&(otherTrie->rootNode),&compare1);
    compare(&(otherTrie->rootNode),&rootNode,&compare2);
    return compare1 and compare2;
}

void TrieTree::compare(trieNode **currentNode,trieNode **otherCurrentNode,bool *comparison){
    //Si en alguna llamada son diferentes
    if((*comparison) == false)
        return;
    
    //Si en alguna llamada ambos árboles estan vacíos
    if((*currentNode) == NULL and (*otherCurrentNode) != NULL){
        (*comparison) = false;
        return;
    }
    
    if((*currentNode) != NULL and (*otherCurrentNode) == NULL){
        (*comparison) = false;
        return;
    }

    if((*currentNode) == NULL and (*otherCurrentNode) == NULL){
        (*comparison) = true;
        return;
    }else{        
        if((*currentNode)->value != (*otherCurrentNode)->value){
            (*comparison) = false;
            return;
        }
        //Si no es nodo hoja, explorar la siguiente dimension
        if((*currentNode)->nextDim != NULL){
            if((*otherCurrentNode)->nextDim != NULL)
                compare(&(*currentNode)->nextDim,&(*otherCurrentNode)->nextDim,comparison);
            else{
                (*comparison) = false;
                return;
            }
        }else//Es nulo en el primer trie
            if((*otherCurrentNode)->nextDim != NULL){ //Si no es nulo el otro trie
                (*comparison) = false;
                return;
            }
        
        if((*comparison) == false)
            return;

        if((*currentNode)->nextTrieNode != NULL){//Explorar en la dimension actual
            if((*otherCurrentNode)->nextTrieNode != NULL)
                compare(&(*currentNode)->nextTrieNode,&(*otherCurrentNode)->nextTrieNode,comparison);
            else{
                (*comparison) = false;
                return;
            }
        }else
            if((*otherCurrentNode)->nextTrieNode != NULL){
                (*comparison) = false;
                return;
            }
        
        if((*comparison) == false)
            return;
    }
    (*comparison) = true;
}

TrieTree *TrieTree::clone(){
    trieNode *copyRootNode = NULL;
    trieNode *prevNode = NULL;
    trieNode *copyPrevNode = NULL;

    clone(&prevNode,&rootNode,&copyPrevNode,&copyRootNode);
    TrieTree *newTrie = new TrieTree(copyRootNode);
    return newTrie;
}

void TrieTree::clone(trieNode **prevNode,trieNode **currentNode,trieNode **copyPrevNode,trieNode **copyCurrentNode){
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
            clone(currentNode,&(*currentNode)->nextDim,copyCurrentNode,&(*copyCurrentNode)->nextDim);

        if((*currentNode)->nextTrieNode != NULL){//Explorar en la dimension actual
            clone(currentNode,&(*currentNode)->nextTrieNode,copyCurrentNode,&(*copyCurrentNode)->nextTrieNode);
        }
    }
}

void TrieTree::removeVertex(double *key){
    trieNode * prevNode = NULL;
    removeVertex(&prevNode,&rootNode,key,0);
}

bool TrieTree::removeVertex(trieNode **prevNode,trieNode **currentNode,double *key,int currentDim){
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
bool TrieTree::existsVertex(double * inputKey,int length){
    return existsVertex(&rootNode,inputKey,length,0);
}

/*Método Principal Recursivo para verificar que un vertice exista en un árbol trie.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Apuntador al vector del vertice.
 * -Tamaño del vector.
 * -Dimension Actual.
*/
bool TrieTree::existsVertex(trieNode **currentNode,double * inputKey,int length,int currentDim){
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

//TrieTree* TrieTree::mergeXOR(TrieTree* otherTrie){
//    //Se considera que esta operacion se realizara con EVMs de la misma dimension.
//    if(rootNode == NULL and otherTrie->rootNode != NULL)
//        return otherTrie->clone();
//   
//    if(rootNode != NULL and otherTrie->rootNode == NULL)
//        return clone();
//
//    if(rootNode == NULL and otherTrie->rootNode == NULL){
//        TrieTree *xorTrie= new TrieTree();
//        return xorTrie;
//    }
//    
//    return XOR(otherTrie);
//}

TrieTree * TrieTree::XOR(TrieTree *otherTrie){
    int dim = dimDepth();
    TrieTree *xorTrie = clone();

    double *key = new double[dim];  //ELIMINAR

    //Se realiza la inserción de los elementos del segundo trie en el clon del primero.
    XOR(&xorTrie,&(otherTrie->rootNode),&key,0);

    delete key;

    return xorTrie;
}

void TrieTree::XOR(TrieTree **resultTrie,trieNode **currentNode,double **key,int dim){
    if((*currentNode) == NULL){//Si el arbol esta vacio
        //cout<<vectorToString(key,dim)<<'\n';
        (*resultTrie)->insertVertex((*key),dim);
        return;
    }

    (*key)[dim] = (*currentNode)->value;

    XOR(resultTrie,&(*currentNode)->nextDim,key,dim+1);

    if((*currentNode)->nextTrieNode != NULL)//Explorar en la dimension actual
        XOR(resultTrie,&(*currentNode)->nextTrieNode,key,dim);
}

void TrieTree::putCouplet2(TrieTree* couplet){
    (*coupletIndex) = couplet->rootNode;
    coupletIndex = &((*coupletIndex)->nextTrieNode);
}

// - ANALIZAR SI SE PUEDE MEJORAR EL RENDIMIENTO CON EL COUPLETINDEX
void TrieTree::putCouplet(TrieTree * couplet){
    trieNode * prevNode = NULL;
    putCouplet(&prevNode,&rootNode,couplet->rootNode);
}

/**
 * Poner el couplet al final del Trie
 * @param prevNode
 * @param currentNode
 * @param coupletRoot
 */
void TrieTree::putCouplet(trieNode** prevNode,trieNode **currentNode,trieNode *coupletRoot){
    // - Si se llega al final del Trie.
    if((*currentNode) == NULL){
        if((*prevNode) != NULL)//AL menos hay un nodo en el Trie
            (*prevNode)->nextTrieNode = coupletRoot;
        else//El trie esta vacio
            *currentNode = coupletRoot;
            
        return;
    }
    
    // - Explorar sobre la primera dimension
    putCouplet(currentNode,&((*currentNode)->nextTrieNode),coupletRoot);
}


TrieTree *TrieTree::readCouplet(){
    //SI SE ELIMINA EL EVM RETORNADO, SE ELIMINARA DEL EVM ORIGINAL
    if(!endTrie()){
        TrieTree *couplet = new TrieTree((*coupletIndex)->nextDim);
        coupletIndex = &((*coupletIndex)->nextTrieNode);
        return couplet;
    }else
        return NULL;
}

void TrieTree::setCoord(double coord){
    //CLONE?????
    //trieNode* coupletTrie = cloneTrie();
    trieNode* coupletRoot = new trieNode;
    coupletRoot->value = coord;
    coupletRoot->nextTrieNode = NULL;
    coupletRoot->nextDim = rootNode;
    rootNode = coupletRoot;
}

double TrieTree::getCoord(){
    return rootNode->value;
}

double TrieTree::coupletCoord(){
    return (*coupletIndex)->value;
}

/**
 * Operacion Union para dos Tries 1D
 * @param section1
 * @param section2
 * @return 
 */
TrieTree *TrieTree::unionOperation(TrieTree* section1, TrieTree* section2){
    // Variante de la solucion para la operacion union
    TrieTree *result = new TrieTree();
    trieNode *subSection1 = section1->rootNode, *subSection2 = section2->rootNode;
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
void TrieTree::unionOperation(trieNode* section1, trieNode* section2,TrieTree **result){
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

TrieTree* TrieTree::intersectionOperation(TrieTree *section1, TrieTree *section2){
    // Variante de la solucion para la operacion union
    trieNode *resultTrie = NULL;
    trieNode *subSection1 = section1->rootNode, *subSection2 = section2->rootNode;
    // Manejo de los segmentos que se van formando al realizar la operación: a--b c--d
    trieNode **currentSegment = &resultTrie, *opResult;

    while(subSection1 != NULL and subSection2 != NULL){
        if(subSection1->value <= subSection2->value){
            intersectionOperation(subSection1,subSection2,&opResult);
            
            // Solo si huno una interseccion con el segundo argumento
            if(opResult != NULL){
                mergeSegments(&currentSegment,opResult);

                // verificar si hay interseccion del segmento actual de subSection1 
                // con el siguiente segmento de subSection2
                if(subSection2->nextTrieNode->nextTrieNode != NULL and subSection1->nextTrieNode->value > subSection2->nextTrieNode->nextTrieNode->value){
                    //Moverse al siguiente segmento en subSection2
                    subSection2 = subSection2->nextTrieNode->nextTrieNode;
                }else{
                    // verificar si hay interseccion del segmento actual de subSection2 
                    // con el siguiente segmento de subSection1
                    if(subSection1->nextTrieNode->nextTrieNode != NULL and subSection2->nextTrieNode->value > subSection1->nextTrieNode->nextTrieNode->value){
                        //Moverse al siguiente segmento en subSection1
                        subSection1 = subSection1->nextTrieNode->nextTrieNode;
                    }else{
                        subSection2 = subSection2->nextTrieNode->nextTrieNode;
                        subSection1 = subSection1->nextTrieNode->nextTrieNode;
                    }
                }
            }else{
//                subSection2 = subSection2->nextTrieNode->nextTrieNode;
                subSection1 = subSection1->nextTrieNode->nextTrieNode;
            }
        }else{
            intersectionOperation(subSection2,subSection1,&opResult);

            // Solo si huno una interseccion con el segundo argumento
            if(opResult != NULL){
                mergeSegments(&currentSegment,opResult);

                // verificar si hay interseccion del segmento actual de subSection2
                // con el siguiente segmento de subSection1
                if(subSection1->nextTrieNode->nextTrieNode != NULL and subSection2->nextTrieNode->value > subSection1->nextTrieNode->nextTrieNode->value){
                    //Moverse al siguiente segmento en subSection1
                    subSection1 = subSection1->nextTrieNode->nextTrieNode;
                }else{
                    // verificar si hay interseccion del segmento actual de subSection1
                    // con el siguiente segmento de subSection2
                    if(subSection2->nextTrieNode->nextTrieNode != NULL and subSection1->nextTrieNode->value > subSection2->nextTrieNode->nextTrieNode->value){
                        //Moverse al siguiente segmento en subSection2
                        subSection2 = subSection2->nextTrieNode->nextTrieNode;
                    }else{
                        subSection1 = subSection1->nextTrieNode->nextTrieNode;
                        subSection2 = subSection2->nextTrieNode->nextTrieNode;
                    }
                }
            }else{
                subSection2 = subSection2->nextTrieNode->nextTrieNode;
//                subSection1 = subSection1->nextTrieNode->nextTrieNode;            
            }
        }
    }
    // Cuando se llega al final de alguno o ambos, ya no es necesario hacer operación alguna
   
//    cout<<"UnionTrieResult: \n";
//    result->printTrie();
    TrieTree *result = new TrieTree(resultTrie);
    
    return result;
}

void TrieTree::intersectionOperation(trieNode* section1, trieNode* section2,trieNode **result){
    // Los vertices estan ordenados de menor a mayor.
    double a,b,c,d;
    
    // Validaciones para el caso de que uno o ambos sean nulos
    if(section1 == NULL or section2 == NULL){
//        cout<<"Caso 0.1: Ambos Nulos...\n";
        *result = NULL;
        return;
    }
    
    // Para una situacion normal
    // Tomando a--b del EVM resultante (primer argumento)
    a = section1->value, b = section1->nextTrieNode->value; // Arista a---b
    c = section2->value, d = section2->nextTrieNode->value; // Arista c---d
    
    // Caso 1: las aristas son disjuntas
    // Aparentemente solo se presentara el caso: a--b c--d
    if(b <= c){
//        cout<<"Caso 1: las aristas son disjuntas\n";
        *result = NULL;
        return;
    }
    
    // Las aristas coinciden
    if(a == c  and b == d){
//        cout<<"Caso 2: las aristas coinciden\n";
        trieNode *firstTrie = new trieNode;
        firstTrie->value = a;
        firstTrie->nextDim = NULL;
        
        trieNode *secondTrie = new trieNode;
        secondTrie->value = b;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;

        *result = firstTrie;
        return;
    }
    
    // Las aristas se incluyen
    if(a <= c  and d <= b){
//        cout<<"Caso 3: las aristas se incluyen\n";
        trieNode *firstTrie = new trieNode;
        firstTrie->value = c;
        firstTrie->nextDim = NULL;
        
        trieNode *secondTrie = new trieNode;
        secondTrie->value = d;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;

        *result = firstTrie;
        return;
    }

    // Superposición
    if(a <= c  and b <= d){
//        cout<<"Caso 4: Superposicion\n";
        trieNode *firstTrie = new trieNode;
        firstTrie->value = c;
        firstTrie->nextDim = NULL;
        
        trieNode *secondTrie = new trieNode;
        secondTrie->value = b;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;

        *result = firstTrie;

        return;
    }
}

TrieTree *TrieTree::differenceOperation(TrieTree* section1, TrieTree* section2){
    // Variante de la solucion para la operacion union
    trieNode *resultTrie = NULL;
    trieNode *subSection1 = section1->rootNode, *subSection2 = section2->rootNode;
    trieNode **currentSegment= &resultTrie,*opResult = NULL;

    // Para el caso de la UNION se considera a result vacio inicialmente   
    // Mientras ninguno de los dos llegue al final
    while(subSection1 != NULL and subSection2 != NULL){
        differenceOperation(&subSection1,&subSection2,&opResult);
        mergeSegments(&currentSegment,opResult);
    }
    
    while(subSection1 != NULL){
        trieNode *firstTrie = new trieNode;
        firstTrie->value = (subSection1)->value;
        firstTrie->nextDim = NULL;

        trieNode *secondTrie = new trieNode;
        secondTrie->value = (subSection1)->nextTrieNode->value;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;
        opResult = firstTrie;
        subSection1 = ((subSection1)->nextTrieNode->nextTrieNode);
        mergeSegments(&currentSegment,opResult);
    }
//    cout<<"UnionTrieResult: \n";
//    result->printTrie();
    
    TrieTree *result = new TrieTree(resultTrie);
    
    return result;
}

void TrieTree::differenceOperation(trieNode** section1, trieNode** section2,trieNode **result){
    
    if((*section1) == NULL){
        *result = NULL;
        return;
    }
    
    if((*section1) != NULL and *section2 == NULL){
//        cout<<"Caso 0.1: Ambos Nulos...\n";
        
        trieNode *firstTrie = new trieNode;
        firstTrie->value = (*section1)->value;
        firstTrie->nextDim = NULL;

        trieNode *secondTrie = new trieNode;
        secondTrie->value = (*section1)->nextTrieNode->value;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;
        *result = firstTrie;
        *section1 = ((*section1)->nextTrieNode->nextTrieNode);
        return;
    }
    
    // Disjuntos A--B C--D
    if((*section1)->nextTrieNode->value <= (*section2)->value){
        trieNode *firstTrie = new trieNode;
        firstTrie->value = (*section1)->value;
        firstTrie->nextDim = NULL;

        trieNode *secondTrie = new trieNode;
        secondTrie->value = (*section1)->nextTrieNode->value;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;
        *result = firstTrie;
        *section1 = ((*section1)->nextTrieNode->nextTrieNode);
        return;
    }
    
    // Disjuntos C--D A--B
    if((*section1)->value >= (*section2)->nextTrieNode->value){
        *section2 = ((*section2)->nextTrieNode->nextTrieNode);
        
        // Buscar una interseccion
        while(*section2 != NULL){
            // C--A--D--B
            if((*section2)->value <= (*section1)->value and (*section2)->nextTrieNode->value > (*section1)->value){
                break;
            }

            // (A,C)--(B,D)
            if((*section2)->value >= (*section1)->value and (*section2)->value < (*section1)->nextTrieNode->value){
                break;
            }
            
            // Se llego al caso A--B C--D
            if((*section2)->value >= (*section1)->nextTrieNode->value){
                break;
            }
            
            *section2 = ((*section2)->nextTrieNode->nextTrieNode);
        }
        
        differenceOperation(section1, section2,result);
        return;
    }
    
    // Coincidentes (AC)--(BD)
    if((*section1)->value == (*section2)->value and (*section1)->nextTrieNode->value == (*section2)->nextTrieNode->value){
        *result = NULL;
        *section1 = ((*section1)->nextTrieNode->nextTrieNode);
        *section2 = ((*section2)->nextTrieNode->nextTrieNode);
        return;
    }
    
    // Inclusivos 1 A<C AND D<B
    if((*section1)->value < (*section2)->value and (*section2)->nextTrieNode->value < (*section1)->nextTrieNode->value){
        //cout<<"Inclusivos 1 A<C AND D<B\n";
        //A
        trieNode *firstTrie = new trieNode;
        firstTrie->value = (*section1)->value;
        firstTrie->nextDim = NULL;
        //C
        trieNode *secondTrie = new trieNode;
        secondTrie->value = (*section2)->value;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;
        
        //D
        trieNode *thirdTrie = new trieNode;
        thirdTrie->value = (*section2)->nextTrieNode->value;
        thirdTrie->nextDim = NULL;
        //B
        trieNode *fourthTrie = new trieNode;
        fourthTrie->value = (*section1)->nextTrieNode->value;
        fourthTrie->nextDim = NULL;
        fourthTrie->nextTrieNode = NULL;

        thirdTrie->nextTrieNode = fourthTrie;
        
        //secondTrie->nextTrieNode = thirdTrie;
        *section2 = ((*section2)->nextTrieNode->nextTrieNode);
        trieNode *tempResult = NULL;
        trieNode *tempSection1 = thirdTrie;
        differenceOperation(&tempSection1, section2,&tempResult);
        
        if(tempResult != NULL){
            deleteTrie(thirdTrie);
            secondTrie->nextTrieNode = tempResult;
        }else{
            secondTrie->nextTrieNode = thirdTrie;
        }
        
        *result = firstTrie;
        *section1 = ((*section1)->nextTrieNode->nextTrieNode);
        return;
    }
 
    // Inclusivos 2 A==C AND D<B
    if((*section1)->value == (*section2)->value and (*section2)->nextTrieNode->value < (*section1)->nextTrieNode->value){
        //D
        trieNode *firstTrie = new trieNode;
        firstTrie->value = (*section2)->nextTrieNode->value;
        firstTrie->nextDim = NULL;
        //B
        trieNode *secondTrie = new trieNode;
        secondTrie->value = (*section1)->nextTrieNode->value;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;
        
        //*result = firstTrie;
        *section2 = ((*section2)->nextTrieNode->nextTrieNode);
        trieNode *tempResult = NULL;
        trieNode *tempSection1 = firstTrie;
        differenceOperation(&tempSection1, section2,&tempResult);
        
        if(tempResult != NULL){
            deleteTrie(firstTrie);
            *result = tempResult;
        }else{
            *result = firstTrie;
        }

        *section1 = ((*section1)->nextTrieNode->nextTrieNode);
        return;
    }

    // Inclusivos 3 A<C AND D==B
    if((*section1)->value < (*section2)->value and (*section2)->nextTrieNode->value == (*section1)->nextTrieNode->value){
        //A
        trieNode *firstTrie = new trieNode;
        firstTrie->value = (*section1)->value;
        firstTrie->nextDim = NULL;
        //C
        trieNode *secondTrie = new trieNode;
        secondTrie->value = (*section2)->value;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;
        
        *result = firstTrie;
        *section2 = ((*section2)->nextTrieNode->nextTrieNode);
        *section1 = ((*section1)->nextTrieNode->nextTrieNode);
//        trieNode *aux = firstTrie;        
//        differenceOperation(&aux, section2,&((*result)->nextTrieNode->nextTrieNode));
        return;
    }

    // Inclusivos Otro sentido C<=A AND D>=B
    if((*section2)->value <= (*section1)->value and (*section2)->nextTrieNode->value >= (*section1)->nextTrieNode->value){
        *result = NULL;
        *section1 = ((*section1)->nextTrieNode->nextTrieNode);
        return;
    }

    // Superpuestos A<C AND B<D
    if((*section1)->value < (*section2)->value and (*section1)->nextTrieNode->value < (*section2)->nextTrieNode->value){
        //A
        trieNode *firstTrie = new trieNode;
        firstTrie->value = (*section1)->value;
        firstTrie->nextDim = NULL;
        //C
        trieNode *secondTrie = new trieNode;
        secondTrie->value = (*section2)->value;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;
        
        *result = firstTrie;
        *section1 = ((*section1)->nextTrieNode->nextTrieNode);
        return;
    }

    // Superpuestos Otro sentido C<A AND D<B
    if((*section2)->value < (*section1)->value and (*section2)->nextTrieNode->value < (*section1)->nextTrieNode->value){
        //D
        trieNode *firstTrie = new trieNode;
        firstTrie->value = (*section2)->nextTrieNode->value;
        firstTrie->nextDim = NULL;
        //C
        trieNode *secondTrie = new trieNode;
        secondTrie->value = (*section1)->nextTrieNode->value;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;
        
        //*result = firstTrie;
        *section2 = ((*section2)->nextTrieNode->nextTrieNode);
        trieNode *tempResult = NULL;
        trieNode *tempSection1 = firstTrie;
        differenceOperation(&tempSection1, section2,&tempResult);
        
        if(tempResult != NULL){
            deleteTrie(firstTrie);
            *result = tempResult;
        }else{
            *result = firstTrie;
        }

        *section1 = ((*section1)->nextTrieNode->nextTrieNode);
        return;
    }
}

TrieTree* TrieTree::xorOperation(TrieTree* section1, TrieTree* section2){
    trieNode *resultTrie = NULL, **currentSegment,*opResult = NULL;
    trieNode *subSection1 = section1->rootNode, *subSection2 = section2->rootNode;
    currentSegment= &resultTrie;
    
    // Para el caso de la UNION se considera a result vacio inicialmente   
    // Mientras ninguno de los dos llegue al final
    xorOperation(subSection1,subSection2,&opResult);
    mergeSegments(&currentSegment,opResult);
    while(subSection1 != NULL and subSection2 != NULL){
        // Si Hay interseccion el nodo final del resultado
        if(subSection1->nextTrieNode->nextTrieNode == NULL)
            subSection1 = subSection1->nextTrieNode->nextTrieNode;
        else
            if((*currentSegment) != NULL and 
                    subSection1->nextTrieNode->nextTrieNode->value <= (*currentSegment)->nextTrieNode->value){
                subSection1 = subSection1->nextTrieNode->nextTrieNode;
                xorOperation(*currentSegment,subSection1,&opResult);
                deleteTrie(*currentSegment);
                *currentSegment = NULL;
                mergeSegments(&currentSegment,opResult);
            }else{
                if(subSection2->nextTrieNode->nextTrieNode == NULL)
                    subSection2 = subSection2->nextTrieNode->nextTrieNode;
                else
                    if((*currentSegment) != NULL and 
                            subSection2->nextTrieNode->nextTrieNode->value <= (*currentSegment)->nextTrieNode->value){
                        subSection2 = subSection2->nextTrieNode->nextTrieNode;
                        xorOperation(*currentSegment,subSection2,&opResult);
                        deleteTrie(*currentSegment);
                        *currentSegment = NULL;
                        mergeSegments(&currentSegment,opResult);
                    }else{
                        subSection1 = subSection1->nextTrieNode->nextTrieNode;
                        xorOperation(*currentSegment,subSection1,&opResult);
                        deleteTrie(*currentSegment);
                        *currentSegment = NULL;
                        mergeSegments(&currentSegment,opResult);

                        subSection2 = subSection2->nextTrieNode->nextTrieNode;
                        xorOperation(*currentSegment,subSection2,&opResult);
                        deleteTrie(*currentSegment);
                        *currentSegment = NULL;
                        mergeSegments(&currentSegment,opResult);
                    }
            }
    }
    
    while(subSection1 != NULL){
        subSection1 = subSection1->nextTrieNode->nextTrieNode;
        xorOperation(*currentSegment,subSection1,&opResult);
        deleteTrie(*currentSegment);
        *currentSegment = NULL;
        mergeSegments(&currentSegment,opResult);
    }

    while(subSection2 != NULL){
        subSection2 = subSection2->nextTrieNode->nextTrieNode;
        xorOperation(*currentSegment,subSection2,&opResult);
        deleteTrie(*currentSegment);
        *currentSegment = NULL;
        mergeSegments(&currentSegment,opResult);
    }
//    cout<<"UnionTrieResult: \n";
//    result->printTrie();
    TrieTree *result = new TrieTree(resultTrie);
    return result;
}

void TrieTree::xorOperation(trieNode* section1, trieNode* section2,trieNode **result){
    
    if(section1 != NULL and section2 == NULL){
        *result = cloneSegment(section1);
        return;
    }
    
    if(section1 == NULL and section2 != NULL){
        *result = cloneSegment(section2);
        return;
    }

    if(section1 == NULL and section2 == NULL){
        *result = NULL;
        return;
    }

    // Disjuntos A--B C--D
    if(section1->nextTrieNode->value < section2->value){
//        cout<<"Disjuntos A--B C--D\n";
        *result = cloneSegment(section1);        
        (*result)->nextTrieNode->nextTrieNode = cloneSegment(section2);
        return;
    }

    // Disjuntos C--D A--B
    if(section2->nextTrieNode->value < section1->value){
//        cout<<"Disjuntos C--D A--B\n";
        *result = cloneSegment(section2);        
        (*result)->nextTrieNode->nextTrieNode = cloneSegment(section1);
        return;
    }

    //Contiguos A--BC--D
    if(section1->nextTrieNode->value == section2->value){
//        cout<<"Contiguos A--BC--D\n";
        //A
        trieNode *firstNode= new trieNode;
        firstNode->value = section1->value;
        firstNode->nextDim = NULL;
        //D
        trieNode *secondNode = new trieNode;
        secondNode->value = section2->nextTrieNode->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;

        *result = firstNode;        
        return;
    }

    //Contiguos C--DA--B
    if(section2->nextTrieNode->value == section1->value){
//        cout<<"Contiguos C--DA--B\n";
        //C
        trieNode *firstNode= new trieNode;
        firstNode->value = section2->value;
        firstNode->nextDim = NULL;
        //B
        trieNode *secondNode = new trieNode;
        secondNode->value = section1->nextTrieNode->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;
        *result = firstNode;        
        return;
    }

    
    // Coincidentes (AC)--(BD)
    if(section1->value == section2->value and section1->nextTrieNode->value == section2->nextTrieNode->value){
//        cout<<"Coincidentes (AC)--(BD)\n";
        *result = NULL;
        return;
    }
    
    //Inclusivos A--C--D--B
    // Inclusivos 1 A<C AND D<B
    if(section1->value < section2->value and section2->nextTrieNode->value < section1->nextTrieNode->value){
//        cout<<"Inclusivos A--C--D--B 1 A<C AND D<B\n";
        //A
        trieNode *firstNode = new trieNode;
        firstNode->value = section1->value;
        firstNode->nextDim = NULL;
        //C
        trieNode *secondNode = new trieNode;
        secondNode->value = section2->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;
        
        //D
        trieNode *thirdNode = new trieNode;
        thirdNode->value = section2->nextTrieNode->value;
        thirdNode->nextDim = NULL;
        //B
        trieNode *fourthNode = new trieNode;
        fourthNode->value = section1->nextTrieNode->value;
        fourthNode->nextDim = NULL;
        fourthNode->nextTrieNode = NULL;

        thirdNode->nextTrieNode = fourthNode;
        secondNode->nextTrieNode = thirdNode;
        *result = firstNode;
        return;
    }
 
    //Inclusivos A--C--D--B
    // Inclusivos 2 A==C AND D<B
    if(section1->value == section2->value and section2->nextTrieNode->value < section1->nextTrieNode->value){
//        cout<<"Inclusivos A--C--D--B 2 A==C AND D<B\n";
        //D
        trieNode *firstNode = new trieNode;
        firstNode->value = section2->nextTrieNode->value;
        firstNode->nextDim = NULL;
        //B
        trieNode *secondNode = new trieNode;
        secondNode->value = section1->nextTrieNode->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;
        
        *result = firstNode;
        return;
    }
    
    //Inclusivos A--C--D--B
    // Inclusivos 3 A<C AND D==B
    if(section1->value < section2->value and section2->nextTrieNode->value == section1->nextTrieNode->value){
//        cout<<"Inclusivos A--C--D--B 3 A<C AND D==B\n";
        //A
        trieNode *firstNode = new trieNode;
        firstNode->value = section1->value;
        firstNode->nextDim = NULL;
        //C
        trieNode *secondNode = new trieNode;
        secondNode->value = section2->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;
        
        *result = firstNode;
        return;
    }

    //Inclusivos 2 C--A--B--D
    // Inclusivos 1 C<A AND B<D
    if(section2->value < section1->value and section1->nextTrieNode->value < section2->nextTrieNode->value){
//        cout<<"Inclusivos C--A--B--D 1 C<A AND B<D\n";
        //c
        trieNode *firstNode = new trieNode;
        firstNode->value = section2->value;
        firstNode->nextDim = NULL;
        //A
        trieNode *secondNode = new trieNode;
        secondNode->value = section1->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;
        
        //B
        trieNode *thirdNode = new trieNode;
        thirdNode->value = section1->nextTrieNode->value;
        thirdNode->nextDim = NULL;
        //D
        trieNode *fourthNode = new trieNode;
        fourthNode->value = section2->nextTrieNode->value;
        fourthNode->nextDim = NULL;
        fourthNode->nextTrieNode = NULL;

        thirdNode->nextTrieNode = fourthNode;
        secondNode->nextTrieNode = thirdNode;

        *result = firstNode;
        return;
    }
 
    //Inclusivos 2 C--A--B--D
    // Inclusivos 2 C==A AND B<D
    if(section1->value == section2->value and section1->nextTrieNode->value < section2->nextTrieNode->value){
//        cout<<"Inclusivos C--A--B--D 2 C==A AND B<D\n";
        //B
        trieNode *firstNode = new trieNode;
        firstNode->value = section1->nextTrieNode->value;
        firstNode->nextDim = NULL;
        //D
        trieNode *secondNode = new trieNode;
        secondNode->value = section2->nextTrieNode->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;
        
        *result = firstNode;
        return;
    }
    
    //Inclusivos 2 C--A--B--D
    // Inclusivos 3 C<A AND B==D
    if(section2->value < section1->value and section2->nextTrieNode->value == section1->nextTrieNode->value){
//        cout<<"Inclusivos C--A--B--D 3 C<A AND B==D\n";
        //C
        trieNode *firstNode = new trieNode;
        firstNode->value = section2->value;
        firstNode->nextDim = NULL;
        //A
        trieNode *secondNode = new trieNode;
        secondNode->value = section1->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;
        
        *result = firstNode;
        return;
    }

    //Superpuestos A--C--B--D
    if(section1->value < section2->value and section1->nextTrieNode->value < section2->nextTrieNode->value){
//        cout<<"Superpuestos A--C--B--D\n";
        //A
        trieNode *firstNode = new trieNode;
        firstNode->value = section1->value;
        firstNode->nextDim = NULL;
        //C
        trieNode *secondNode = new trieNode;
        secondNode->value = section2->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;
        
        //B
        trieNode *thirdNode = new trieNode;
        thirdNode->value = section1->nextTrieNode->value;
        thirdNode->nextDim = NULL;
        //D
        trieNode *fourthNode = new trieNode;
        fourthNode->value = section2->nextTrieNode->value;
        fourthNode->nextDim = NULL;
        fourthNode->nextTrieNode = NULL;

        thirdNode->nextTrieNode = fourthNode;
        secondNode->nextTrieNode = thirdNode;
        
        *result = firstNode;
        return;
    }    

    //Superpuestos 2 C--A--D--B
    if(section2->value < section1->value and section2->nextTrieNode->value < section1->nextTrieNode->value){
//        cout<<"Superpuestos 2 C--A--D--B\n";
        //C
        trieNode *firstNode = new trieNode;
        firstNode->value = section2->value;
        firstNode->nextDim = NULL;
        //A
        trieNode *secondNode = new trieNode;
        secondNode->value = section1->value;
        secondNode->nextDim = NULL;
        secondNode->nextTrieNode = NULL;

        firstNode->nextTrieNode = secondNode;
        
        //D
        trieNode *thirdNode = new trieNode;
        thirdNode->value = section2->nextTrieNode->value;
        thirdNode->nextDim = NULL;
        //B
        trieNode *fourthNode = new trieNode;
        fourthNode->value = section1->nextTrieNode->value;
        fourthNode->nextDim = NULL;
        fourthNode->nextTrieNode = NULL;

        thirdNode->nextTrieNode = fourthNode;
        secondNode->nextTrieNode = thirdNode;
        
        *result = firstNode;
        return;
    }    
}


void TrieTree::mergeSegments(trieNode ***currentSegment, trieNode *otherSegment){
    // Si es el nodo raiz
    if(otherSegment == NULL)
        return;
    
    if(**currentSegment == NULL){
        **currentSegment = otherSegment;
        while((**currentSegment)->nextTrieNode->nextTrieNode != NULL ){
            *currentSegment = &((**currentSegment)->nextTrieNode->nextTrieNode);
        }
    }else{
        (**currentSegment)->nextTrieNode->nextTrieNode = otherSegment;
        *currentSegment = &((**currentSegment)->nextTrieNode->nextTrieNode);
        while((**currentSegment)->nextTrieNode->nextTrieNode != NULL ){
            *currentSegment = &((**currentSegment)->nextTrieNode->nextTrieNode);
        }
    }
}

trieNode *TrieTree::cloneSegment(trieNode *segment){
        trieNode *firstTrie = new trieNode;
        firstTrie->value = segment->value;
        firstTrie->nextDim = NULL;

        trieNode *secondTrie = new trieNode;
        secondTrie->value = segment->nextTrieNode->value;
        secondTrie->nextDim = NULL;
        secondTrie->nextTrieNode = NULL;

        firstTrie->nextTrieNode = secondTrie;
        
        return firstTrie;
}


void TrieTree::condInsertVertex(double * inputKey,int length){
    if(!existsVertex(inputKey,length))
        insertVertex(inputKey, length);
}


void TrieTree::printTrie(){
    int dim = dimDepth();
    double * testKey = new double[dim];
    printTrie(rootNode,&testKey,0);
    
    delete testKey;
}

void TrieTree::printTrie(trieNode *currentNode,double **key, int dim){
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

double TrieTree::size(){
    double trieSize = 0.0;
    if((&rootNode) == NULL){   //que el arbol no este vacio
        return 0.0;
    }
    size(rootNode,&trieSize);
    return trieSize;
}

/**
 * Método Principal para obtener el Tamaño (número de nodos hoja) de un árbol Trie.
 * @param currentNode: Nodo actual en la exploración.
 * @param size: Contador del tamaño.
 */
void TrieTree::size(trieNode *currentNode,double *trieSize){
    //Si se llega a un nodo hoja
    if((currentNode) == NULL){
        *trieSize = (*trieSize) +1;
        return;
    }

    size((currentNode)->nextDim,trieSize);

    if((currentNode)->nextTrieNode != NULL){
        size((currentNode)->nextTrieNode,trieSize);
    }
}

int TrieTree::dimDepth(){
    return dimDepth(rootNode,0);
}

/**
 * Metodo que devuelve la profundidad dimensional de un trie.
 * @param currentNode
 * @param dim
 * @return 
 */
int TrieTree::dimDepth(trieNode* currentNode,int dim){
    if(currentNode == NULL)
        return dim;
    
    return dimDepth(currentNode->nextDim,dim+1);
}

string TrieTree::vectorToString(double **vector,int size){
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
string TrieTree::vectorToString2(double **vector,int size){
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
