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
}

nDEVM::nDEVM(trieNode *node) {
    rootNode = node;
}

nDEVM::nDEVM(const nDEVM& orig) {
}

nDEVM::~nDEVM() {   //METODO PARA ELIMINAR EL ARBOL TRIE
}

/*Método Auxiliar para Insertar un vértice en un árbol trie.
 * Argumentos:
 * -Nodo raíz.
 * -Apuntador al vector de entrada
 * -Tamaño del vector.
 */
void nDEVM::insertVertex(double * inputKey,int length){
    trieNode * prevNode = NULL;
    insertVertex(&prevNode,&rootNode,inputKey,length,0,length);
}

/*Método Auxiliar para Insertar un vértice en un árbol trie.
 * Argumentos:
 * -Nodo raíz.
 * -Apuntador al vector de entrada
 * -Tamaño del vector.
 */
void nDEVM::insertVertex(trieNode **otherRootNode,double * inputKey,int length){
    trieNode * prevNode = NULL;
    insertVertex(&prevNode,otherRootNode,inputKey,length,0,length);
}


/*Método Principal Recursivo para Insertar un vértice en un árbol trie.
 * Se utilizan contadores para determinar si se alcanzó la profundidad máxima
 */
bool nDEVM::insertVertex(trieNode **prevNode,trieNode **currentNode,double * inputKey,int length,int currentDim,int matchCount){
    //currentDim inicia desde cero, es la posicion de la dimension actual en el array
    if(!(currentDim < length))
        if(matchCount == 0)
            return true;   //El vertice ya existe
        else
            return false;    //El vertice es nuevo
    
    if(*currentNode == NULL){
        trieNode *node = new trieNode;
        node->value = inputKey[currentDim];
        node->dimDepth = currentDim;
        node->nextDim = NULL;
        node->nextTrieNode = NULL;
        
        *currentNode = node;
        //Que no sea el nodo raiz
        if(*prevNode != NULL)
            //Cuando se inserta un nodo en la misma dimension
            if((*prevNode)->dimDepth == currentDim)
                (*prevNode)->nextTrieNode = *currentNode;
            else
                (*prevNode)->nextDim = *currentNode;
        //Moverse a la siguiente dimension
        insertVertex(currentNode,&((*currentNode)->nextDim),inputKey,length, currentDim+1,matchCount);
    }else{
        //Si ya existe un nodo con el mismo valor en esta dimension
        if((*currentNode)->value == inputKey[currentDim]){
            bool vertexStatus = insertVertex(currentNode,&((*currentNode)->nextDim),inputKey,length, currentDim+1,matchCount-1);
            //Si el vertice ya existe, se intenta eliminar
            if(vertexStatus){
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
        }else    
            //Busqueda por comparacion
            if((*currentNode)->value > inputKey[currentDim]){
                //Si hay un nodo en la misma dimension con un valor mayor
                //Insertar nuevo nodo antes de dicho nodo
                trieNode *node = new trieNode;
                node->value = inputKey[currentDim];
                node->dimDepth = currentDim;
                node->nextDim = NULL;
                node->nextTrieNode = (*currentNode);
                //Si se llega por profundidad
                if((*prevNode)->nextDim == (*currentNode)){
                    (*prevNode)->nextDim = node;                
                }else{
                    (*prevNode)->nextTrieNode = node;                
                }                

                insertVertex(&node,&node->nextDim,inputKey,length, currentDim+1,matchCount);
            }else
                //Se explora otro nodo en la misma dimension
                insertVertex(currentNode,&((*currentNode)->nextTrieNode),inputKey,length, currentDim,matchCount);
    }
}

/*Método Auxiliar para Imprimir en consola el contenido de un árbol trie.
 * Argumentos:
 * -Nodo raíz.
*/
void nDEVM::printTrie(){
    double * testKey = new double[4];
    printTrie(rootNode,&testKey,0);
}

/*Método Principal Recursivo para Imprimir en consola el contenido de un árbol trie.
 * Argumentos:
 * -Nodo raíz.
 * -Apuntador doble, para almacenar los elementos del trie
 * -Dimension actual
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

/*Método Principal Recursivo para obtener el Tamaño de un árbol trie.
 * Argumentos:
 * -Nodo raíz.
 * -apuntador al tamaño del arbol
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
bool nDEVM::compareEVM(nDEVM *otherEVM){
    bool compare1 = true, compare2 = true;
    compareTrie(&rootNode,&(otherEVM->rootNode),&compare1);
    compareTrie(&(otherEVM->rootNode),&rootNode,&compare2);
    return compare1 and compare2;
}

/*Método Principal Recursivo para comparar dos árboles trie.
 * Argumentos:
 * -Nodo raíz del primer arbol.
 * -Nodo raíz del segundo arbol.
 * -Apuntador a la variable con el valor de comparación.
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

/*Método Principal Recursivo para clonar un árbol trie.
 * Argumentos:
 * -Nodo previo del arbol original.
 * -Nodo raíz del arbol original.
 * -Nodo previo del arbol copia.
 * -Nodo raíz del arbol copia.
*/
void nDEVM::cloneTrie(trieNode **prevNode,trieNode **currentNode,trieNode **copyPrevNode,trieNode **copyCurrentNode){
    if((*currentNode) == NULL){//Si el arbol esta vacio
        return;
    }else{
        trieNode *node = new trieNode;
        node->dimDepth = (*currentNode)->dimDepth;
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
        if(removeVertex(currentNode,&(*currentNode)->nextDim,key,currentDim+1))
        {
            //Si se llega por profundidad
            if((*prevNode)->nextDim == (*currentNode)) 
            {
                //Si no hay mas nodos a la derecha
                if((*currentNode)->nextTrieNode == NULL)
                {
                    delete *currentNode;
                    *currentNode = NULL;
                    return true;    //Intentar eliminar nodo padre
                }else
                {
                    trieNode *node = (*currentNode)->nextTrieNode;
                    delete *currentNode;
                    *currentNode = NULL;
                    (*prevNode)->nextDim = node;//Que apunte al siguiente nodo en la misma dimension
                    return false;   //Nodo padre no puede ser eliminado
                }
            }else
            {
                //Si se llega por amplitud, busqueda en la misma dimension
                //No hay mas nodos a la derecha
                if((*currentNode)->nextTrieNode == NULL)    
                {
                    delete *currentNode;
                    *currentNode = NULL;
                }else
                {
                    trieNode *node = (*currentNode)->nextTrieNode;
                    delete *currentNode;
                    *currentNode = NULL;
                    (*prevNode)->nextTrieNode= node;//Que apunte al siguiente nodo en la dimension actual
                }
                //Nodo padre no puede ser eliminado
                return false;
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
nDEVM* nDEVM::XOR(nDEVM* otherEVM,int dim){
    nDEVM *xorEVM = new nDEVM(XORTrie(&(otherEVM->rootNode),dim));
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
                        populate3DVoxel(&newKey);
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

/*Método para generar e insertar la voxelización que consiste de 8 vértices, generada a partir del vértice en el origen.
 * Argumentos:
 * -Nodo Raiz.
 * -Apuntador doble del vector de entrada.
*/
void nDEVM::populate3DVoxel(double **inputKey){
    populate3DVoxel(inputKey,3,0);
}

/*Método Principal y Recursivo para generar e insertar la voxelización que consiste de 8 vértices, generada a partir del vértice en el origen.
 * Argumentos:
 * -Nodo Raiz.
 * -Apuntador doble del vector de entrada.
*/
void nDEVM::populate3DVoxel(double **inputKey,int dim,int currentDim){
    if(!(currentDim < dim)){
        insertVertex(*inputKey,4);
        //cout<<vectorToString(&inputKey,dim+1)<<endl;
        return;
    }
    populate3DVoxel(inputKey,dim,currentDim+1);
    (*inputKey)[currentDim+1] = (*inputKey)[currentDim+1]+1;
    populate3DVoxel(inputKey,dim,currentDim+1);
    (*inputKey)[currentDim+1] = (*inputKey)[currentDim+1]-1;
}

/*Método para vaciar un arbol trie en un archivo de texto .evm
 * Argumentos:
 * -Nodo Raiz.
*/
void nDEVM::EVMFile(){
    double * testKey = new double[4];   //Eliminar
    ofstream outputFile( "EVMFile.evm" );
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