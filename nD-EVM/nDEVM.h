/* 
 * File:   nDEVM.h
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
#include <cmath>
#include "BMP.h"

#include "TrieTree.h"

#ifndef NDEVM_H
#define	NDEVM_H

using namespace std;

template<typename valueType>
class nDEVM {
private:
public:
    // -- Arbol Trie del nD-EVM actual.
    TrieTree<valueType> *trieTree;
    // -- Maximo y minimo de una mascara en las dimensiones x,y,t utilizado
    // -- para evaluar contra la longitud de una animacion o frame.
    valueType maskMax[3];
    valueType maskMin[3];
    // -- Contactos internos minimos y maximos de una mascara.
    valueType LcMin = 0, LcMax = 0;
    // -- Objeto de la clase SOM.
    SOM *som;
    
    // -- Constructores
    // - Constructor de un EVM vacio
    nDEVM();
    // - Constructor de un EVM a partir de un arbol Trie.
    nDEVM(TrieTree<valueType> *trie);
    nDEVM(const nDEVM& orig);
    virtual ~nDEVM();
    
    // -- Metodo basicos
    // - Obtener el nodo raiz del arbol Trie
    trieNode<valueType> *getRootNode();
    // - Evaluar si el EVM actual esta vacio.
    bool isEmpty();
    // - Devuelve la cantidad de vertices extremos que contiene el EVM actual.
    valueType size();
    // - Imprime en consola el contenido del EVM actual.
    void printEVM();
    string vectorToString(valueType **vector,int size);
    string vectorToString2(valueType **vector,int size);


    // - Inserta un vertice al EVM actual.
    void insertVertex(valueType * inputKey,int length);
    // - Compara el EVM actual contra otro EVM dado.
    bool compareEVM(nDEVM *otherEVM);
    // - Retorna un EVM que es un clon del EVM actual.
    nDEVM *cloneEVM();
    // - Elimina un vertice dado del EVM actual.
    void removeVertex(valueType *key);
    // - Evalua si un vertice dado existe en el EVM actual.
    bool existsVertex(valueType * inputKey,int length);
    // - Realiza la operacion XOR entre el EVM actual y otro EVM dado.
    nDEVM* mergeXOR(nDEVM *otherEVM);
    
    // --- Metodos utilizados para casos especificos y ciertas pruebas.
    bool compareByCouplets(nDEVM *otherEVM);
    void rawFileToEVM(string fileName,int x1,int x2,int x3);
    void populate3DVoxel(valueType **inputKey);
    void populate2DVoxel(valueType **inputKey);
    void populateVoxel(valueType **inputKey,int dim,int currentDim,int offset);
    void populateVoxel(valueType **inputKey,int dim,int currentDim);
    
    void populateFrameVoxel(valueType **voxelBase,int frameDim,int currentDim,
        valueType ** voxelLengths);

    void EVMFile(int index);
    void EVMFile(string suffix, int index);
    void EVMFile(string folder,string suffix, int index);
    void EVMFile(ofstream *EVMFile,trieNode<valueType> *currentNode,valueType **key, int dim);
    
    
    void putCouplet(nDEVM * couplet);
//    void putCouplet(trieNode** prevNode,trieNode **currentNode,trieNode *coupletRoot);
    void putSection(nDEVM * section);
    
    void setCoord(valueType coord);
    valueType getCoord();
    valueType coupletCoord();
    
    void resetCoupletIndex();
    nDEVM* readCouplet();
    nDEVM* readSection();
    bool endEVM();
    nDEVM* getSection(nDEVM* section, nDEVM *couplet);
    nDEVM* getCouplet(nDEVM* section1, nDEVM *section2);
    nDEVM* EVMSectionSequence();
    void EVMSectionSequence(nDEVM** sectionSequence);
    nDEVM* EVMCoupletSequence();
    void EVMCoupletSequence(nDEVM** sectionSequence);
    
    int dimDepth();
    int getDimDepth(trieNode<valueType> * currentNode,int dim);

//-- Operaciones regularizadas
    nDEVM* booleanOperation(nDEVM* evm2,string op);
    nDEVM<valueType> *booleanOperation(nDEVM* evm2, string op,int dim);
    nDEVM* booleanOperation(nDEVM *evm1, nDEVM* evm2, string op, int n);
    nDEVM* booleanOperation(nDEVM *section1, nDEVM *section2, string op);
    void nextObject(nDEVM *p, nDEVM *q,valueType *coord,bool *fromP, bool *fromQ);
    // Operacion basica entre dos segmentos 1D.
//    void generalUnionOperation(trieNode* section1, trieNode* section2,nDEVM **result);
    void condInsertVertex(valueType * inputKey,int length);
    nDEVM* unionOperation(nDEVM* section1, nDEVM* section2);
    void unionOperation(trieNode<valueType>* segment1, trieNode<valueType>* segment2,nDEVM **result);
    bool putCoupletByOp(string op,int argPosition);
    nDEVM* intersectionOperation(nDEVM* section1, nDEVM* section2);
    void intersectionOperation(trieNode<valueType>* section1, trieNode<valueType>* section2,trieNode<valueType> **result);
    void mergeSegments(trieNode<valueType> ***currentSegment, trieNode<valueType> *otherSegment);
    nDEVM* differenceOperation(nDEVM* section1, nDEVM* section2);
    void differenceOperation(trieNode<valueType> ** section1, trieNode<valueType>** section2,trieNode<valueType> **result);
    nDEVM* xorOperation(nDEVM* section1, nDEVM* section2);
    void xorOperation(trieNode<valueType>* section1, trieNode<valueType>* section2,trieNode<valueType> **result);
    trieNode<valueType> *cloneSegment(trieNode<valueType> *segment);
    //Pruebas de operaciones Booleanas
    void load3DRawFile(string fileName,int voxelSize);
    void load2DRawFile(string fileName,int voxelSize);
    void loadnDRawFile(string fileName,int voxelSize,int dim);
    void voxelizeRawFile(valueType **voxelInput,ifstream *inputFile,int voxelSize,int dim, int currentDim);
    
    // -- Contenido
    valueType content();
    valueType content(nDEVM **p, int n);    
    valueType length();
    valueType boundaryContent();
    valueType boundaryContent(nDEVM *p, int n);
    valueType perimeter();
    double discreteCompactness(valueType lMin,valueType lMax);
    void dcNormalization(int _parts,int _dcFiles,double max,double min);  // - Agregar un prefijo
    void dcNormalization(int _parts,int _dcFiles);
    nDEVM * dimLeftShift();
    valueType internalContacts();
    valueType internalContacts(nDEVM * p,int n);
    valueType totalInternalContacts();
    
    // - Cargar Videos Imagenes
    void loadImageFile(string fileName);
    void loadImage(string fileName);
    void generateAnimation(string framePrefix, valueType initFrame,
        valueType endFrame);
    void generateAnimation(string framePrefix, valueType endFrame);
    void frameSequence(int initFrame,int endFrame);
    void frameSequence(int endCouplet);
    void maskInit(int xLength, int yLength, int timeLength,
        int colorComponents, int colorCompSize);
    void minMask(int xLength, int yLength, int timeLength,
        int colorComponents);
    void populateMask(valueType **voxelInput,int maskDim,int currentDim,
        valueType ** maskLengths);
    nDEVM<valueType> * maskIntersection(nDEVM* mask,int initCouplet,int endCouplet);
    void animNextObject(valueType iCouplet, nDEVM *mask,valueType *coord,
        bool *fromP, bool *fromQ);
    void EVMTraslation(int dim,valueType shift);
    void maskDimReset(int dim);
    void maskAnimConv(nDEVM * mask,int initFrame,int endFrame,
        valueType _xMax,valueType _yMax);
    nDEVM<valueType> *maskIntersection(nDEVM* mask, nDEVM* sectionSeq);
    nDEVM<valueType> *maskAnimSections(nDEVM* mask,int endCouplet);
    void maskAnimConv2(nDEVM * mask,int endFrame,
        valueType _xMax,valueType _yMax);
    void dcContent(int _part,int _dc);
    
    void saveEVM(string fileName,int index);
    void saveEVM2(string fileName,int index);
    void readEVM(string fileName);
    void readEVM2(string fileName);
    
    // - SOM Clustering
//    void subAnimClustering(int clusters,int _dcParts,int _dcFiles);
    void subAnimClustering(int clusters,int _dcParts, int _iter);
    void clusterContent(int cluster);
    void clusterContent(int cluster, nDEVM * mask,int initFrame,int endFrame,
            valueType _xMax,valueType _yMax);
    void clusterContent2(int cluster, nDEVM * mask,int endFrame,
        valueType _xMax,valueType _yMax);
    void clusterContent3(int cluster, nDEVM * mask,int endFrame,
        valueType _width,valueType _length);
    
    
    // - Guardar imagenes de los EVMs de los frames.
    void frameMaskInit(int colorComponents, int colorCompSize);
    valueType dimMax(int dim);
    void frameToImage(int width, int height,int colorCount,string imageName);
    void animImages(valueType _width, valueType _length,string imageName);
    void clusterFrame(int cluster, string fileName,int idx,valueType _width,
        valueType _length);
};

#endif	/* TRIETREE_H */


template<typename valueType> 
nDEVM<valueType>::nDEVM() {
//    rootNode = NULL;
//    resetCoupletIndex();
    trieTree = new TrieTree<valueType>();
}

template<typename valueType> 
nDEVM<valueType>::nDEVM(TrieTree<valueType> *trie) {
    trieTree = trie;
//    rootNode = node;
//    resetCoupletIndex();
}

template<typename valueType> 
nDEVM<valueType>::nDEVM(const nDEVM& orig) {
}

template<typename valueType> 
nDEVM<valueType>::~nDEVM() {   //METODO PARA ELIMINAR EL ARBOL TRIE
    delete trieTree;
    //trieTree->deleteTrie();
}

template<typename valueType> 
void nDEVM<valueType>::resetCoupletIndex(){
    trieTree->resetCoupletIndex();
}

template<typename valueType> 
bool nDEVM<valueType>::isEmpty(){
    return trieTree->isEmpty();
}


/*Método Auxiliar para Insertar un vértice en un árbol trie.
 * Argumentos:
 * -Nodo raíz.
 * -Apuntador al vector de entrada
 * -Tamaño del vector.
 */
template<typename valueType> 
void nDEVM<valueType>::insertVertex(valueType * inputKey,int length){
    trieTree->insertVertex(inputKey,length);
}

/**
 * Método Auxiliar para Imprimir en consola el contenido de un árbol Trie.
 */
template<typename valueType> 
void nDEVM<valueType>::printEVM(){
    trieTree->printTrie();
}

/*Método Auxiliar para obtener el Tamaño de un árbol trie.
 * Argumentos:
 * -Nodo raíz.
*/
template<typename valueType> 
valueType nDEVM<valueType>::size(){
    return trieTree->size();
}

/**
 * Método para comparar dos EVMs.
 * @param otherEVM: El EVM que será comparado con el EVM actual.
 * @return Se retorna un valor booleano (true, false).
 */
template<typename valueType> 
bool nDEVM<valueType>::compareEVM(nDEVM *otherEVM){
    return trieTree->compare(otherEVM->trieTree);
}

/**
 * Método para comparar dos EVMs considerando los couplets sobre la primera dimensión
 * @param otherEVM: El EVM que será comparado con el EVM actual.
 * @return Se retorna un valor booleano (true, false).
 */
template<typename valueType> 
bool nDEVM<valueType>::compareByCouplets(nDEVM *otherEVM){
    bool totalCompare;
    nDEVM* currentCouplet1 = new nDEVM();
    nDEVM* currentCouplet2 = new nDEVM();
    
    while(!(endEVM()) and !(otherEVM->endEVM())){
        currentCouplet1 = readCouplet();
        currentCouplet2 = otherEVM->readCouplet();
        totalCompare = currentCouplet1->compareEVM(currentCouplet2);
//        compareTrie(&(currentCouplet2->rootNode),&(currentCouplet1->rootNode),&compare2);
//        totalCompare = compare1 and compare2;
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
 * Método para clonar un EVM.
 * @return nDEVM *, apuntador al nuevo EVM.
 */
template<typename valueType> 
nDEVM<valueType> * nDEVM<valueType>::cloneEVM(){
    TrieTree<valueType> *copyTrie = trieTree->clone();
    nDEVM *cloneEVM = new nDEVM(copyTrie);
    copyTrie = NULL;
    return cloneEVM;
}

/*Método Auxiliar para remover un vertice de un árbol trie.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Apuntador al vector del elemento a eliminar.
*/
template<typename valueType> 
void nDEVM<valueType>::removeVertex(valueType *key){
    trieTree->removeVertex(key);
}

/*Método Auxiliar para verificar que un vertice exista en un árbol trie.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Apuntador al vector del vertice.
 * -Tamaño del vector.
*/
template<typename valueType> 
bool nDEVM<valueType>::existsVertex(valueType * inputKey,int length){
    return trieTree->existsVertex(inputKey,length);
}

/**
 * Método para realizar la operación XOR de dos EVMs.
 * @param otherEVM: El otro EVM.
 * @return El un nuevo EVM obtenido al operar los dos EVMs.
 */
template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::mergeXOR(nDEVM* otherEVM){
    //Se considera que esta operacion se realizara con EVMs de la misma dimension.
    if(isEmpty() and !(otherEVM->isEmpty()))
        return otherEVM->cloneEVM();
   
    if(!isEmpty() and otherEVM->isEmpty())
        return cloneEVM();

    if(isEmpty() and otherEVM->isEmpty()){
//        nDEVM *xorEVM = new nDEVM();
        return new nDEVM();
    }
    
    TrieTree<valueType> *resultTrie = trieTree->mergeXOR(otherEVM->trieTree);
    nDEVM<valueType> *resultEVM = new nDEVM<valueType>(resultTrie);
    resultTrie = NULL;
    return resultEVM;
}

/*Método para vaciar un archivo .raw a un trie.
 * Argumentos:
 * -Nombre del archivo.
 * -Dimensiones.
*/
template<typename valueType> 
void nDEVM<valueType>::rawFileToEVM(string fileName,int x1,int x2,int x3){
    //const char * file = "VL-vismale-(128x256x256)-(1.5,1,1).raw";
    ifstream fileInput;
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file

    unsigned char buffer;
    valueType *newKey = new valueType[4]; //ELIMINAR
    valueType value;
    
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
                        newKey[0] = (valueType)0;
                        populate3DVoxel(&newKey);   //Corregir: Insertar en un metodo populate4DVoxel
                        newKey[0] = (valueType)(value+1);
                        populate3DVoxel(&newKey);
                    }
                }
            }
        }

        fileInput.close();
    }else
        cout << "No se abrio correctamente el archivo: "<< fileName << "\n";
    
    cout << "Se finalizo la lectura del archivo: "<< fileName << "\n";
    delete [] newKey;
}

/**
 * Cargar un archivo binario, que representa una voxelizacion en nD, en el EVM...
 * @param fileName: Nombre del archivo...
 * @param voxelSize: Tamaño de la voxelización...
 * @param dim: Dimensión del espacio.
 */
template<typename valueType> 
void nDEVM<valueType>::loadnDRawFile(string fileName,int voxelSize,int dim){
    ifstream inputFile;
    inputFile.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file

    
    valueType *newVoxel = new valueType[dim]; //ELIMINAR
    
    if(!inputFile.is_open()){
        cout << "No se pudo abrir el archivo: "<< fileName << "\n";
        return;
    }
    
    voxelizeRawFile(&newVoxel,&inputFile,voxelSize,dim,dim);
    
    delete [] newVoxel;
    inputFile.close();
}

/**
 * Generar la Voxelización mediante la exploración y el contenido (en bytes) del archivo.
 * @param voxelInput: Vector que contiene las coordenadas del voxel.
 * @param inputFile: Apuntador al archivo de entrada.
 * @param voxelSize: Tamaño de la voxelización.
 * @param dim: Dimensión del espacio.
 * @param currentDim: Dimensión actual en la exploración
 */
template<typename valueType> 
void nDEVM<valueType>::voxelizeRawFile(valueType **voxelInput,ifstream *inputFile,int voxelSize,int dim, int currentDim){
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

/*Método para generar e insertar la voxelización que consiste de 8 vértices, generada a partir del vértice en el origen.
 * Argumentos:
 * -Nodo Raiz.
 * -Apuntador doble del vector de entrada.
*/
template<typename valueType> 
void nDEVM<valueType>::populate3DVoxel(valueType **voxelInput){
    populateVoxel(voxelInput,3,0,1);
}

/*Método Principal y Recursivo para generar e insertar la voxelización que consiste de 8 vértices, generada a partir del vértice en el origen.
 * Argumentos:
 * -Nodo Raiz.
 * -Apuntador doble del vector de entrada.
*/
template<typename valueType> 
void nDEVM<valueType>::populateVoxel(valueType **voxelInput,int dim,int currentDim,int offset){
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

/**
 * Metodo para hacer la voxelizacion de pixeles con cualquier cantidad de 
 * componentes de color
 * @param voxelInput: Arreglo que contiene la informacion del pixel.
 * @param frameDim: Dimension de la base de la voxelizacion para un Frame (2D)
 * @param currentDim: Dimension actual en las ejecuciones recursivas...
 * @param colors: Componentes de color
 */
template<typename valueType> 
void nDEVM<valueType>::populateFrameVoxel(valueType **voxelBase,int frameDim,int currentDim,
        valueType ** voxelLengths){
    if(!(currentDim < frameDim)){
        insertVertex(*voxelBase,frameDim);
//        cout<<vectorToString(voxelBase,frameDim)<<endl;
        return;
    }
    populateFrameVoxel(voxelBase,frameDim,currentDim+1,voxelLengths);
    (*voxelBase)[currentDim] = (*voxelBase)[currentDim] + (*voxelLengths)[currentDim];
    populateFrameVoxel(voxelBase,frameDim,currentDim+1,voxelLengths);
    (*voxelBase)[currentDim] = (*voxelBase)[currentDim] - (*voxelLengths)[currentDim];
}

/**
 * Generar e insertar un Voxel
 * @param voxelInput: las primeras posiciones son las coordenadas del voxel, la ultima posicion es el contenido del voxel
 * @param dim: La dimension del voxel
 * @param currentDim: La dimension actual
 */
template<typename valueType> 
void nDEVM<valueType>::populateVoxel(valueType **voxelInput,int dim,int currentDim){
    if(!(currentDim < dim)){
        insertVertex(*voxelInput,dim+1);
//        cout<<vectorToString(voxelInput,dim+1)<<endl;
        return;
    }
    populateVoxel(voxelInput,dim,currentDim+1);
    (*voxelInput)[currentDim] = (*voxelInput)[currentDim]+1;
    populateVoxel(voxelInput,dim,currentDim+1);
    (*voxelInput)[currentDim] = (*voxelInput)[currentDim]-1;
}

/*Método para vaciar un arbol trie en un archivo de texto .evm
 * Argumentos:
 * -Nodo Raiz.
*/
template<typename valueType> 
void nDEVM<valueType>::EVMFile(int index){
    EVMFile("",index);
}

/**
 * Metodo para guardar un archivo 3D-EVM de texto...
 * @param suffix
 * @param index
 */
template<typename valueType> 
void nDEVM<valueType>::EVMFile(string suffix, int index){
    valueType * testKey = new valueType[3];   //Eliminar
    // ***
//    string fileName = "EVMFiles/EVM_"+suffix+to_string(index)+".evm";
    string fileName = "..\\EVMFiles\\EVM_"+suffix+to_string(index)+".evm";
    
    ofstream outputFile( fileName );
    if ( ! outputFile.is_open() ){    
        cout << "El archivo no se pudo abrir!" << '\n';    
        return;
    } 
    outputFile<<"XYZ"<<'\n'<<'3'<<'\n';
    EVMFile(&outputFile,trieTree->rootNode,&testKey,0);
    outputFile.close();
    delete [] testKey;
}

template<typename valueType> 
void nDEVM<valueType>::EVMFile(string folder,string suffix, int index){
    valueType * testKey = new valueType[3];   //Eliminar
    string fileName = folder+"/EVM_"+suffix+to_string(index)+".evm";
    
    ofstream outputFile( fileName );
    if ( ! outputFile.is_open() ){    
        cout << "El archivo no se pudo abrir!" << '\n';    
        return;
    } 
    outputFile<<"XYZ"<<'\n'<<'3'<<'\n';
    EVMFile(&outputFile,trieTree->rootNode,&testKey,0);
    outputFile.close();
    delete [] testKey;
}

/*Método Principal y Recursivo para vaciar un arbol trie en un archivo de texto .evm
 * Argumentos:
 * -APuntador al archivo en donde se almacena la información.
 * -Nodo Raiz.
 * -Apuntador doble al vector que se usa para obtener la información
 * -Dimension.
*/
template<typename valueType> 
void nDEVM<valueType>::EVMFile(ofstream *outputFile,trieNode<valueType> *currentNode,valueType **key, int dim){
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
template<typename valueType> 
string nDEVM<valueType>::vectorToString(valueType **vector,int size){
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
template<typename valueType> 
string nDEVM<valueType>::vectorToString2(valueType **vector,int size){
    string output="";
    char s[5];
    for(int i =0;i<size;i++){
        if(i != 0)
            output+=' ';
        sprintf(s,"%.1f", (double)(*vector)[i]);
        output+=to_string(s);
    }
    return output;
}

/*
 *Metodo auxiliar para agregar un Couplet perpendicular a la dimension x1:
 * Argumentos:
 * -El (n-1)D EVM del couplet.
 */
template<typename valueType> 
void nDEVM<valueType>::putCouplet(nDEVM * couplet){
    trieTree->putCouplet2(couplet->trieTree);
//    putCouplet(&prevNode,&rootNode,couplet->rootNode);
}

/*
 *Metodo auxiliar para agregar una Seccion perpendicular a la dimension x1:
 * Argumentos:
 * -El (n-1)D EVM del couplet.
 */
template<typename valueType> 
void nDEVM<valueType>::putSection(nDEVM * section){
    trieTree->putCouplet2(section->trieTree);
}

/**
 * Obtiene el couplet de la primera dimension asociado a la posicion donde
 * actualmente se encuentra el apuntador de couplets coupletIndex.
 * @return 
 * nDEVM*
 */
template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::readCouplet(){
    //SI SE ELIMINA EL EVM RETORNADO, SE ELIMINARA DEL EVM ORIGINAL
//    nDEVM *coupletEVM = new nDEVM(trieTree->readCouplet());
    return new nDEVM<valueType>(trieTree->readCouplet());
}

/**
 * Obtiene el couplet de la primera dimension asociado a la posicion donde
 * actualmente se encuentra el apuntador de couplets coupletIndex.
 * @return 
 * nDEVM*
 */
template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::readSection(){
    return readCouplet();
}

/**
 * Metodo que indica si se ha alcanzado el ultimo couplet, en base a la posicion
 * del indice de couplets.
 * @return 
 * Boolean
 */
template<typename valueType> 
bool nDEVM<valueType>::endEVM(){
    return trieTree->endTrie();
}

/**
 * Metodo que inserta un nuevo nodo en la dimension x1 con el valor dado, y 
 * el nuevo nodo sera el nodo raiz.
 * @param coord
 * @return 
 */
template<typename valueType> 
void nDEVM<valueType>::setCoord(valueType coord){
    trieTree->setCoord(coord);
}

/**
 * Metodo que devuelve el valor de la primera dimension del couplet asociado al
 * nodo raiz.
 * @return 
 */
template<typename valueType> 
valueType nDEVM<valueType>::getCoord(){
    return trieTree->getCoord();
}

/**
 * Devuelve la operación mergeXOR entre una sección y un couplet, con lo que se
 * obtiene la siguiente sección...
 * @param section
 * @param couplet
 * @return Un nuevo EVM resultante.
 */
template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::getSection(nDEVM* section, nDEVM *couplet){
    return section->mergeXOR(couplet);
}

/**
 * Método que realiza la operación mergeXOR entre dos secciones, con lo que se
 * se obtiene el couplet que se encuentra entre ambas secciones...
 * @param section1
 * @param section2
 * @return Un nuevo EVM resultante.
 */
template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::getCouplet(nDEVM* section1, nDEVM *section2){
    return section1->mergeXOR(section2);
}

template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::EVMSectionSequence(){
    nDEVM* sectionSequence = new nDEVM();
    EVMSectionSequence(&sectionSequence);
    return sectionSequence;
}

template<typename valueType> 
void nDEVM<valueType>::EVMSectionSequence(nDEVM** sectionSequence){
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
        
        currentSection->EVMFile("Section",i);
        //cout<<endl;
        
        // Siguiente Iteración
        prevSection = currentSection;
        currentCouplet = readCouplet();
        i++;
    }
    currentSection->setCoord(i - 1);
    (*sectionSequence)->putSection(currentSection);
    (*sectionSequence)->resetCoupletIndex();
    resetCoupletIndex();
}

template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::EVMCoupletSequence(){
    nDEVM* coupletSequence = new nDEVM();
    EVMCoupletSequence(&coupletSequence);
    return coupletSequence;
}

template<typename valueType> 
void nDEVM<valueType>::EVMCoupletSequence(nDEVM** coupletSequence){
    nDEVM* currentCouplet = new nDEVM();
    nDEVM* prevSection = new nDEVM();
    nDEVM* currentSection = readSection();
    int i = 0;
    while(true){
        currentCouplet = getSection(prevSection,currentSection);
        //cout<<"Couplet "<<i<<": "<<endl;
        //currentCouplet->printTrie();
        
        // Procesamiento
        currentCouplet->EVMFile("Couplet",i);
        currentCouplet->setCoord(i);
        (*coupletSequence)->putCouplet(currentCouplet);

//        currentSection->EVMFile("Couplet",i);
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
    (*coupletSequence)->resetCoupletIndex();
    resetCoupletIndex();
}

/**
 * Metodo que devuelve la profundidad dimensional de un trie.
 * @return 
 */
template<typename valueType> 
int nDEVM<valueType>::dimDepth(){
    return trieTree->dimDepth();
}

/**
 * Método para realizar una operación booleana entre dos EVMs.
 * @param evm1
 * @param evm2
 * @param op
 * @return 
 */
template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::booleanOperation(nDEVM* evm2, string op){
    int dim = dimDepth();
    return booleanOperation(this,evm2,op,dim);
}

template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::booleanOperation(nDEVM* evm2, string op,int dim){
    return booleanOperation(this,evm2,op,dim);
}

template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::booleanOperation(nDEVM *p, nDEVM *q, string op, int n){
    nDEVM<valueType> *pSection,*pPrevSection, *qSection,*qPrevSection, *couplet;
    nDEVM<valueType> *result, *rPrevSection, *rCurrentSection;
    bool fromP, fromQ;
    valueType coord;
    if(n == 1){
        return booleanOperation(p,q,op);
    }
    pSection = new nDEVM<valueType>();
    qSection = new nDEVM<valueType>();
    rCurrentSection = new nDEVM<valueType>();
    result = new nDEVM<valueType>();
    while(!(p->endEVM()) and !(q->endEVM())){
        nextObject(p,q,&coord,&fromP,&fromQ);
        if(fromP){
            couplet = p->readCouplet();
            pPrevSection = pSection;
            pSection = getSection(pSection,couplet);
            delete pPrevSection;    //Liberar Memoria
            delete couplet;

//            cout<<"section fromP, coord: "<<coord<<"\n";
//            pSection->printTrie();
        }
        if(fromQ){
            couplet = q->readCouplet();
            qPrevSection = qSection;
            qSection = getSection(qSection,couplet);
            delete qPrevSection;    //Liberar Memoria
            delete couplet;

//            cout<<"section fromQ, coord: "<<coord<<"\n";
//            qSection->printTrie();
        }

        rPrevSection = rCurrentSection;
        rCurrentSection = booleanOperation(pSection, qSection,op,n-1);
        
//        cout<<"Section Result: \n";
//        rCurrentSection->printTrie();
//        cout<<"\n";
        
        couplet = getCouplet(rPrevSection,rCurrentSection);

//        cout<<"\nCouplet Result: \n";
//        couplet->printTrie();
//        cout<<"---\n";
        
        if(!couplet->isEmpty()){
            couplet->trieTree->isCouplet = true;
            couplet->setCoord(coord);

            result->putCouplet(couplet);
            delete couplet;
        }
        
        delete rPrevSection;    //Liberar Memoria
    }
    while(!(p->endEVM())){
        nDEVM<valueType> *coupletCopy;
//        cout<<"I was Here...\n";
        if(putCoupletByOp(op,1)){
            coord = (*(p->trieTree->coupletIndex))->value;
            couplet = p->readCouplet();
            
            coupletCopy = couplet->cloneEVM();
            coupletCopy->trieTree->isCouplet = true;
            coupletCopy->setCoord(coord);
            
            result->putCouplet(coupletCopy);
            
            delete couplet;
            delete coupletCopy;
        }else
            break;
    }

    while(!(q->endEVM())){
        nDEVM<valueType> *coupletCopy;
//        cout<<"I was Here...\n";
        if(putCoupletByOp(op,2)){
            coord = (*(q->trieTree->coupletIndex))->value;
            couplet = q->readCouplet();

            coupletCopy = couplet->cloneEVM();
            coupletCopy->trieTree->isCouplet = true;
            coupletCopy->setCoord(coord);

            result->putCouplet(coupletCopy);
            delete couplet;
            delete coupletCopy;
        }else
            break;
    }
    p->resetCoupletIndex();
    q->resetCoupletIndex();
    result->resetCoupletIndex();
    
    delete pSection;
    delete qSection;
    delete rCurrentSection;
    
    return result;
}

template<typename valueType> 
void nDEVM<valueType>::nextObject(nDEVM *p, nDEVM *q,valueType *coord,bool *fromP, bool *fromQ){
    if((*(p->trieTree->coupletIndex))->value < (*(q->trieTree->coupletIndex))->value){
        *coord = (*(p->trieTree->coupletIndex))->value;
        *fromP = true;
        *fromQ = false;
        return;
    }

    if((*(q->trieTree->coupletIndex))->value < (*(p->trieTree->coupletIndex))->value){
        *coord = (*(q->trieTree->coupletIndex))->value;
        *fromQ = true;
        *fromP = false;
        return;
    }

    if((*(p->trieTree->coupletIndex))->value == (*(q->trieTree->coupletIndex))->value){
        *coord = (*(p->trieTree->coupletIndex))->value;
        *fromQ = true;
        *fromP = true;
        return;
    }
}

template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::booleanOperation(nDEVM *section1, nDEVM *section2, string op){
    if(op.compare("union") == 0){
        return unionOperation(section1,section2);
    }
    
    if(op.compare("intersection") == 0){
        return intersectionOperation(section1,section2);
    }

    if(op.compare("difference") == 0){
        return differenceOperation(section1,section2);
    }

    if(op.compare("xor") == 0){
        return xorOperation(section1,section2);
    }
    cout<<"La operacion: "<<op<<", no existe...";
    exit(-1);
}

template<typename valueType> 
bool nDEVM<valueType>::putCoupletByOp(string op,int argPosition){
    if(op.compare("union") == 0){
        return true;
    }
    
    if(op.compare("intersection") == 0){
        return false;
    }
    
    if(op.compare("difference") == 0 and argPosition == 1){
        return true;
    }

    if(op.compare("xor") == 0){
        return true;
    }
    
    return false;
}


template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::unionOperation(nDEVM* section1, nDEVM* section2){
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
    
    TrieTree<valueType> *tempTrie = new TrieTree<valueType>();
    TrieTree<valueType> *resultTrie;
    resultTrie = tempTrie->unionOperation(section1->trieTree,section2->trieTree);
    
    nDEVM<valueType> *resultEVM = new nDEVM<valueType>(resultTrie);

    delete tempTrie;

    return resultEVM;
}

template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::intersectionOperation(nDEVM* section1, nDEVM* section2){
    // Casos en el que uno o ambos EVMs estan vacios
    if(section1->isEmpty() or section2->isEmpty()){
        return new nDEVM();
    }
        
    TrieTree<valueType> *tempTrie = new TrieTree<valueType>();
    TrieTree<valueType> *resultTrie;
    resultTrie = tempTrie->intersectionOperation(section1->trieTree,section2->trieTree);
    
    nDEVM<valueType> *result = new nDEVM<valueType>(resultTrie);
    
    delete tempTrie;
    
    return result;
}

template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::differenceOperation(nDEVM* section1, nDEVM* section2){
    // A - B
    if(!section1->isEmpty() and section2->isEmpty()){
        return section1->cloneEVM();
    }
        
    if(section1->isEmpty() and section2->isEmpty()){
        return new nDEVM();
    }
    
    TrieTree<valueType> *tempTrie = new TrieTree<valueType>();
    TrieTree<valueType> *resultTrie;
    resultTrie = tempTrie->differenceOperation(section1->trieTree,section2->trieTree);
    
    nDEVM<valueType> *result = new nDEVM<valueType>(resultTrie);

    delete tempTrie;
    
    return result;
}

template<typename valueType> 
nDEVM<valueType>* nDEVM<valueType>::xorOperation(nDEVM* section1, nDEVM* section2){
    // A xor B
    if(!section1->isEmpty() and section2->isEmpty()){
        return section1->cloneEVM();
    }
        
    if(section1->isEmpty() and !section2->isEmpty()){
        return section2->cloneEVM();
    }

    TrieTree<valueType> *tempTrie = new TrieTree<valueType>();
    TrieTree<valueType> *resultTrie;
    resultTrie = tempTrie->xorOperation(section1->trieTree,section2->trieTree);
    
    nDEVM<valueType> *result = new nDEVM<valueType>(resultTrie);

    delete tempTrie;

    return result;
}

template<typename valueType> 
void nDEVM<valueType>::loadImageFile(string fileName){
    loadImage(fileName);
    return;
}

template<typename valueType> 
void nDEVM<valueType>::loadImage(string fileName){
    BMP bmpImage(fileName);
    int colors = (int)bmpImage.header.bitsPerPixel/8;
    valueType *voxelLengths = new valueType[2+colors]; // - [X,Y,G|(R,G,B)]
    valueType *voxelBase = new valueType[2+colors]; // - [X,Y,G|(R,G,B)]
    unsigned int dataIdx = 0;
    
    cout<<"Loading Frame: "<<fileName<<endl;
    voxelLengths[0] = 1;
    voxelLengths[1] = 1;
    for(int k = 0; k < colors; k++){
        voxelBase[2+k] = 0; // - Las bases para los hyper-voxels
    }
    
    for(int i = 0; i < bmpImage.header.height; i++){
        voxelBase[1] = i;
        
        for(int j = 0; j < bmpImage.header.width*colors; j+= colors){
            voxelBase[0] = (int)(j/colors);
            
//            cout<< <<endl;            
//            populateVoxel(&pixelInfo,2+colorComp-1,0); // - Dim considera la dimension 0 :(
//            populateFrameVoxel(&pixelInfo,2,0,colorComp); // - Dim considera la dimension 0 :(
            for(int k = 0; k < colors; k++){
//                cout<<(int)bmpImage.pImageData[dataIdx]<<',';
                voxelLengths[2+k] = (valueType)(bmpImage.pImageData[dataIdx] + 1 ); // - El color como tal
                dataIdx++;
            }
//            populateVoxel(&pixelInfo,2+colorComp-1,0); // - Dim considera la dimension 0 :(
            populateFrameVoxel(&voxelBase,2+colors,0,&voxelLengths); // - Dim considera la dimension 0 :(
//            cout<<"EVM Content..."<<endl;
//            printEVM();
            // - Red
//            cout<<"RGB: "<<bitset<8>(pixelRGB[0])<<' '<<bitset<8>(pixelRGB[1])<<' '<<bitset<8>(pixelRGB[2])<<endl;
//            bgr |= pixelRGB[0];
//            bgr = bgr << 8;
//            cout<<"R<32>: "<<bitset<32>(bgr)<<endl;
            // - Green
//            bgr |= pixelRGB[1];
//            bgr = bgr << 8;
            // - Blue
//            bgr |= pixelRGB[2];
            
//            cout<<"x: "<<pixelInfo[0]<<"y: "<<pixelInfo[1]<<endl;
//            pixelInfo[2] = 0;
//            populateVoxel(&pixelInfo,2,0);
//            pixelInfo[2] = pixelRGB[0];
//            pixelInfo[3] = pixelRGB[1];
//            pixelInfo[4] = pixelRGB[2];
            // - Almacenando solo la parte Roja, Para GrayScale se tiene el mismo valor para cada color
//            pixelInfo[2] = (valueType)pixelRGB[0]+1;
//            populateVoxel(&pixelInfo,2,0);
            
//            if(j < 350){
//                if(pixelRGB[0] > 0 and pixelRGB[1] > 0 and pixelRGB[2] > 0)
//                    cout<<' ';
//                else
//                    cout<<'*';
//            }
        }
    }
//        cout<<endl;
//    cout<<"Total data: "<<dataIdx<<endl;
    cout<<"Frame loaded..."<<endl;
//    delete [] pixelRGB;
    delete [] voxelLengths;
    delete [] voxelBase;
//    delete bmpImage;
}


/**
 * Metodo para generar una animacion a partir de una secuencia de Frames. Que son una
 * secuencia de imagenes *.bmp. Genera un conjunto de archivos *.evm binarios con 
 * los Couplets de la animacion.
 * @param framePrefix
 * @param initFrame
 * @param endFrame
 * @param _xMax
 * @param _yMax
 */
template<typename valueType> 
void nDEVM<valueType>::generateAnimation(string framePrefix, valueType initFrame,
        valueType endFrame){
    // - Se guarda la animacion sobre el objeto desde que se llama    
    nDEVM<valueType> *currentFrame, *prevFrame,*diffFrame;
//    int time;
    string frameName;
    
    prevFrame = new nDEVM<valueType>();

    for(int i = initFrame; i <= endFrame; i++){
        currentFrame = new nDEVM<valueType>();
//        time = i;

        frameName = framePrefix + to_string(i)+".bmp";
        cout<<"Loading: "<<frameName<<endl;
        
        currentFrame->loadImage(frameName);
        diffFrame = getCouplet(prevFrame,currentFrame);
        diffFrame->saveEVM("frameCouplet",i); 

        delete prevFrame;
        delete diffFrame;
        prevFrame = currentFrame;
    }
    prevFrame->saveEVM("frameCouplet",endFrame+1); 
    delete prevFrame;
    
    resetCoupletIndex();
}

/**
 * Metodo para generar una animacion a partir de una secuencia de Frames. Que son una
 * secuencia de imagenes *.bmp. Genera un conjunto de archivos *.evm binarios con 
 * los Couplets de la animacion.
 * @param framePrefix
 * @param initFrame
 * @param endFrame
 * @param _xMax
 * @param _yMax
 */
template<typename valueType> 
void nDEVM<valueType>::generateAnimation(string framePrefix, valueType endFrame){
    // - Se guarda la animacion sobre el objeto desde que se llama    
    nDEVM<valueType> *currentFrame, *prevFrame,*diffFrame;
    int time;
    prevFrame = new nDEVM<valueType>();
    string frameName;
    
    for(int i = 0; i <= endFrame; i++){
        currentFrame = new nDEVM<valueType>();
        time = i;

        frameName = framePrefix + to_string(i)+".bmp";
//        cout<<"Loading: "<<frameName<<endl;
        
        currentFrame->loadImage(frameName);
        diffFrame = getCouplet(prevFrame,currentFrame);
        diffFrame->saveEVM("frameCouplet",time); 

        delete prevFrame;
        delete diffFrame;
        prevFrame = currentFrame;
    }
    prevFrame->saveEVM("frameCouplet",time+1); 
    delete prevFrame;
//    resetCoupletIndex();
}

/**
 * Obtiene la secuencia original de frames...
 * @param framePrefix
 * @param initCouplet
 * @param endCouplet
 */
template<typename valueType> 
void nDEVM<valueType>::frameSequence(int endCouplet){
    nDEVM<valueType> * currentSection= new nDEVM<valueType>();
    nDEVM<valueType> * prevSection;

    for(int i = 0; i < endCouplet; i++){
        nDEVM<valueType> * currentCouplet = new nDEVM<valueType>();
        currentCouplet->readEVM("frameCouplet" + to_string(i));
        currentCouplet->EVMFile("frameCouplet",i);
        prevSection = currentSection;
        currentSection = getSection(prevSection,currentCouplet);
        currentSection->EVMFile("frameSection",i);
        
        delete prevSection;
        delete currentCouplet;
    }
    delete currentSection;
}

/**
 * Metodo para guardar EVMs en archivos binarios *.evm
 * @param fileName
 * @param index
 */
template<typename valueType> 
void nDEVM<valueType>::saveEVM(string fileName,int index){
    if(index < 0){
        trieTree->saveTrie(fileName);
    }else{
        trieTree->saveTrie(fileName+to_string(index));
    }
}

/**
 * Metodo para guardar EVMs en archivos binarios *.evm en la ruta especificada...
 * @param fileName
 * @param index
 */
template<typename valueType> 
void nDEVM<valueType>::saveEVM2(string fileName,int index){
    if(index < 0){
        trieTree->saveTrie2(fileName);
    }else{
        trieTree->saveTrie2(fileName+to_string(index));
    }
}

/**
 * Lectura de EVMs a partir de arhivos binarios..
 * @param fileName
 */
template<typename valueType> 
void nDEVM<valueType>::readEVM(string fileName){
    trieTree->readTrie(fileName);
}

/**
 * Lectura de EVMs a partir de arhivos binarios, desde la ruta especificada..
 * @param fileName
 */
template<typename valueType> 
void nDEVM<valueType>::readEVM2(string fileName){
    trieTree->readTrie2(fileName);
}

/**
 * Inicializacion de la mascara para segmentacion de video. Las mascaras contienen 
 * LcMax y LcMin para el calculo de DC...
 * @param xLength
 * @param yLength
 * @param timeLength
 * @param colorComponents
 * @param colorCompSize, Tamaño en bytes de cada componente de color. En generar se usa 1 byte.
 */
template<typename valueType>
void nDEVM<valueType>::maskInit(int xLength, int yLength, int timeLength,
        int colorComponents, int colorCompSize){
    maskMin[0] = 0;
    maskMax[0] = timeLength;
    maskMin[1] = 0;
    maskMax[1] = xLength;
    maskMin[2] = 0;
    maskMax[2] = yLength;
    
    valueType colorCompMax = pow(2,colorCompSize*8);
    valueType * maskVoxel = new valueType[3+colorComponents];
    valueType * maskLengths = new valueType[3+colorComponents];
    
    maskVoxel[0] = 0;
    maskLengths[0] = timeLength;
    
    maskVoxel[1] = 0;
    maskLengths[1] = xLength;
    
    maskVoxel[2] = 0;
    maskLengths[2] = yLength;
    
    for(int i = 0; i < colorComponents; i++){
        maskVoxel[3+i] = 0;
        maskLengths[3+i] = colorCompMax;
    }
    populateMask(&maskVoxel,3+colorComponents,0,&maskLengths);
    
    LcMax = totalInternalContacts();
    
    nDEVM<valueType> *otherMask = new nDEVM<valueType>();
    otherMask->minMask(xLength,yLength,timeLength,colorComponents);
    LcMin = otherMask->totalInternalContacts();
    delete [] maskVoxel;
    delete [] maskLengths;
    delete otherMask;
}

template<typename valueType>
void nDEVM<valueType>::minMask(int xLength, int yLength, int timeLength,
        int colorComponents){
    valueType * maskVoxel = new valueType[3+colorComponents];
    valueType * maskLengths = new valueType[3+colorComponents];
    
    maskVoxel[0] = 0;
    maskLengths[0] = timeLength;
    
    maskVoxel[1] = 0;
    maskLengths[1] = xLength;
    
    maskVoxel[2] = 0;
    maskLengths[2] = yLength;
    
    for(int i = 0; i < colorComponents; i++){
        maskVoxel[3+i] = 0;
        maskLengths[3+i] = 1;
    }
    populateMask(&maskVoxel,3+colorComponents,0,&maskLengths);
    delete [] maskVoxel;
    delete [] maskLengths;
}

template<typename valueType> 
void nDEVM<valueType>::populateMask(valueType **maskVoxel,int maskDim,int currentDim,
        valueType ** maskLengths){
    if(!(currentDim < maskDim)){
        insertVertex(*maskVoxel,maskDim);
//        cout<<vectorToString(maskVoxel,maskDim)<<endl;
        return;
    }
    populateMask(maskVoxel,maskDim,currentDim+1,maskLengths);
    (*maskVoxel)[currentDim] = (*maskVoxel)[currentDim] + (*maskLengths)[currentDim];
    populateMask(maskVoxel,maskDim,currentDim+1,maskLengths);
    (*maskVoxel)[currentDim] = (*maskVoxel)[currentDim] - (*maskLengths)[currentDim];
}

template<typename valueType> 
nDEVM<valueType> * nDEVM<valueType>::maskIntersection(nDEVM* mask,int initCouplet,int endCouplet){
    int iCouplet = 0, iCoupletMax = endCouplet - initCouplet;
    
    nDEVM<valueType> *animSection,*animPrevSection, *maskSection,*maskPrevSection, *couplet;
    nDEVM<valueType> *result, *rPrevSection, *rCurrentSection;
    bool fromAnim, fromMask;
    valueType coord;

//    nDEVM* currentFrame = new nDEVM();
    int dim = mask->dimDepth();

    animSection = new nDEVM<valueType>();
    maskSection = new nDEVM<valueType>();
    rCurrentSection = new nDEVM<valueType>();
    
    result = new nDEVM<valueType>();

    while(iCouplet < iCoupletMax and !(mask->endEVM())){
        // - Version modificada de nextObject
        animNextObject(iCouplet,mask,&coord,&fromAnim,&fromMask);
        
        if(fromAnim){
            couplet = new nDEVM<valueType>();
            couplet->readEVM("frameCouplet" + to_string(initCouplet+iCouplet));
            animPrevSection = animSection;
            animSection = getSection(animSection,couplet);
    //        currentFrame->saveEVM("frame",i);
            delete couplet;   
            delete animPrevSection;
        }
        
        if(fromMask){
            couplet = mask->readCouplet();
            maskPrevSection = maskSection;
            maskSection = getSection(maskSection,couplet);
            delete couplet;
            delete maskPrevSection;
        }
        
        rPrevSection = rCurrentSection;
        rCurrentSection = booleanOperation(animSection, maskSection,"intersection",dim-1);

        couplet = getCouplet(rPrevSection,rCurrentSection);
        
        if(!couplet->isEmpty()){
            couplet->trieTree->isCouplet = true;
            couplet->setCoord(coord);

//            result->putCouplet(couplet);
            // El resultado se guarda en el nDEVM actual...
            result->putCouplet(couplet);
            delete couplet;
        }
        
        delete rPrevSection;    //Liberar Memoria
        iCouplet++;
    }
    mask->resetCoupletIndex();
    result->resetCoupletIndex();
//    resetCoupletIndex();
    
    delete animSection;
    delete maskSection;
    delete rCurrentSection;
    
    return result;
}

template<typename valueType> 
nDEVM<valueType> * nDEVM<valueType>::maskIntersection(nDEVM* mask, nDEVM* sectionSeq){
    
    nDEVM<valueType> *animSection,*maskSection,*couplet;
    nDEVM<valueType> *result, *rPrevSection, *rCurrentSection;

    valueType coord;

//    nDEVM* currentFrame = new nDEVM();
    int dim = mask->dimDepth();

//    animSection = new nDEVM<valueType>();
    maskSection = new nDEVM<valueType>();
    rCurrentSection = new nDEVM<valueType>();
    
    result = new nDEVM<valueType>();

    coord = mask->getCoord();
    couplet = mask->readCouplet();
    maskSection = getSection(maskSection,couplet);
    delete couplet;

    while(!sectionSeq->endEVM()){
        // - Version modificada de nextObject
//        animNextObject(iCouplet,mask,&coord,&fromAnim,&fromMask);
        
        animSection = sectionSeq->readCouplet();
        
        rPrevSection = rCurrentSection;
        rCurrentSection = booleanOperation(animSection, maskSection,"intersection",dim-1);

        couplet = getCouplet(rPrevSection,rCurrentSection);
        
        if(!couplet->isEmpty()){
            couplet->trieTree->isCouplet = true;
            couplet->setCoord(coord);

            result->putCouplet(couplet);
            delete couplet;
        }
        
        delete rPrevSection;    //Liberar Memoria
        delete animSection;

        coord++;
    }

    if(!rCurrentSection->isEmpty()){
        rCurrentSection->trieTree->isCouplet = true;
        rCurrentSection->setCoord(coord);

        result->putCouplet(rCurrentSection);
    }

    mask->resetCoupletIndex();
    result->resetCoupletIndex();
    sectionSeq->resetCoupletIndex();

    delete maskSection;
    delete rCurrentSection;

//    while(!result->endEVM()){
//        coord = result->getCoord();
//        cout<<"Result Couplet idx: "<<coord<<endl;
//        animSection = result->readCouplet();
//        animSection->EVMFile("maskInter",coord);
//    }
    
    return result;
}

/**
 * Extraer un EVM con la secuencia de Secciones necesarias para la interseccion
 * de la maskara, todo en base al desplazamiento con respecto al tiempo...
 * @param mask
 * @param initCouplet
 * @param endCouplet
 * @return 
 */
template<typename valueType> 
nDEVM<valueType> * nDEVM<valueType>::maskAnimSections(nDEVM* mask,int endCouplet){
    int iCouplet = 0;
    
    nDEVM<valueType> *animSection,*animPrevSection, *couplet;
    nDEVM<valueType> *sectionSequence;
    bool fromAnim, fromMask,saveSection = false;
    valueType coord;

    animSection = new nDEVM<valueType>();
    
    sectionSequence = new nDEVM<valueType>();

    while(iCouplet <= endCouplet and !(mask->endEVM())){
        // - Version modificada de nextObject
        animNextObject(iCouplet,mask,&coord,&fromAnim,&fromMask);
        
        if(fromAnim){
            couplet = new nDEVM<valueType>();
            couplet->readEVM("frameCouplet" + to_string(iCouplet));
            animPrevSection = animSection;
            animSection = getSection(animPrevSection,couplet);
            delete couplet;   
            delete animPrevSection;
        }
        
        if(fromMask){
            couplet = mask->readCouplet();
            if(!mask->endEVM()){
                saveSection = true;
            }else{
                saveSection = false;
            }
            delete couplet;
        }
        
        if(saveSection){
//            cout<<"Loading Section: "<<iCouplet<<endl;
            nDEVM<valueType> *section;
            section = animSection->cloneEVM();
            section->trieTree->isCouplet = true;
            section->setCoord(iCouplet);
            sectionSequence->putCouplet(section);
            
            delete section;
        }
        
        iCouplet++;
    }
    mask->resetCoupletIndex();
    sectionSequence->resetCoupletIndex();
    
    delete animSection;
    
//    while(!sectionSequence->endEVM()){
//        iSection = sectionSequence->getCoord();
//        cout<<"Section idx: "<<iSection<<endl;
//        animSection = sectionSequence->readCouplet();
//        animSection->EVMFile("animSecSeq",iSection);
//    }
//    sectionSequence->resetCoupletIndex();

    return sectionSequence;
}

template<typename valueType> 
void nDEVM<valueType>::animNextObject(valueType iCouplet, nDEVM *mask,valueType *coord,
        bool *fromP, bool *fromQ){
    if(iCouplet < (*(mask->trieTree->coupletIndex))->value){
        *coord = iCouplet;
        *fromP = true;
        *fromQ = false;
        return;
    }

    if((*(mask->trieTree->coupletIndex))->value < iCouplet){
        *coord = (*(mask->trieTree->coupletIndex))->value;
        *fromQ = true;
        *fromP = false;
        return;
    }

    if(iCouplet == (*(mask->trieTree->coupletIndex))->value){
        *coord = iCouplet;
        *fromQ = true;
        *fromP = true;
        return;
    }
}

/**
 * Traslacion del nDEVM actual en una dimension y un desplazamiento especificos.
 * Solo se considerandesplazamientos en t,x,y.
 * @param dim
 * @param shift
 */
template<typename valueType>
void nDEVM<valueType>::EVMTraslation(int dim,valueType shift){
    trieTree->TrieTranslation(dim,shift);
    maskMax[dim] += shift;
    maskMin[dim] += shift;
}


template<typename valueType>
void nDEVM<valueType>::maskDimReset(int dim){
    trieTree->TrieTranslation(dim,-maskMin[dim]);
    maskMax[dim] -= maskMin[dim];
    maskMin[dim] -= maskMin[dim];
}

template<typename valueType>
void nDEVM<valueType>::maskAnimConv(nDEVM * mask,int initFrame,int endFrame,
        valueType _xMax,valueType _yMax){
    valueType maxFrames = endFrame- initFrame;
    
    nDEVM<valueType> *prevResult;// = new nDEVM<valueType>();
    nDEVM<valueType> *currentResult = new nDEVM<valueType>();
    
    string fileName = "";
    double *dcPtr = new double;
    unsigned int i = 0;
    unsigned int dcFile = 0;
    unsigned int dcPart = mask->getCoord();
    
    fileName = "dcFiles/Part"+to_string(dcPart)+"/dcFile"+to_string(dcFile)+".dc";
    ofstream outputFile( fileName.c_str(),ios_base::out|ios_base::binary );
    if ( ! outputFile.is_open() ){    
        cout << "El archivo: "+fileName+" no se pudo abrir!!" << '\n';    
        return;
    }

    // Recorrido en T
    while(mask->maskMax[0] <= maxFrames){
        cout<<"Computing Part"<<dcPart<<"..."<<endl;
//        cout<<"Mask tMin: "<<mask->maskMin[0]<<", tMax: "<<mask->maskMax[0]<<endl;
        // - Recorrido en y
        while(mask->maskMax[2] <= _yMax){
//            cout<<"Mask yMin: "<<mask->maskMin[2]<<", yMax: "<<mask->maskMax[2]<<endl;
            // - Recorrido en x
            while(mask->maskMax[1] <= _xMax){
//                cout<<"Mask xMin: "<<mask->maskMin[1]<<", xMax: "<<mask->maskMax[1]<<endl;
                
                prevResult = currentResult;
                currentResult = maskIntersection(mask,initFrame,endFrame+1);
                
//                cout<<"subSeq: "<<i<<endl;
//                cout<<"subSeq: "<<i<<", DC: "<<
//                        currentResult->discreteCompactness(mask->LcMin,mask->LcMax)<<endl;

                *dcPtr = currentResult->discreteCompactness(mask->LcMin,mask->LcMax);
                cout<<"subSeq["<<i<<"] => DC: "<<*dcPtr<<endl;
                outputFile.write((char *) dcPtr,sizeof(double));
//                cout<<"subSeq: "<<i<<", Content: "<<
//                        currentResult->content()<<endl;
                
                delete prevResult;

//                animPrevResult = animResult;
//                animResult = animResult->booleanOperation(currentResult,"union");
//                delete animPrevResult;
                mask->EVMTraslation(1,2); // - Traslacion en x
                
                i++;
                if(i >= 2500){
                    outputFile.close();
                    dcFile++;
                    fileName = "dcFiles/Part"+to_string(dcPart)+"/dcFile"+to_string(dcFile)+".dc";
                    outputFile.open( fileName.c_str(),ios_base::out|ios_base::binary );
                    if ( ! outputFile.is_open() ){    
                        cout << "El archivo: "+fileName+" no se pudo abrir!!" << '\n';    
                        return;
                    }
                    i = 0;
                }
            }
            mask->maskDimReset(1); // - reset en x
            mask->EVMTraslation(2,2); // - traslacion en y
        }
        mask->maskDimReset(1); // - reset en x
        mask->maskDimReset(2); // - reset en y
        mask->EVMTraslation(0,1); // - traslacion en tiempo
        
        outputFile.close();
        return;
        
        dcPart = mask->trieTree->rootNode->value;
        dcFile = 0;
        fileName = "dcFiles/Part"+to_string(dcPart)+"/dcFile"+to_string(dcFile)+".dc";
        outputFile.open( fileName.c_str(),ios_base::out|ios_base::binary );
        if ( ! outputFile.is_open() ){    
            cout << "El archivo: "+fileName+" no se pudo abrir!!" << '\n';    
            return;
        }
        i = 0;
    }
    mask->maskDimReset(0); // - reset en tiempo
    delete currentResult;
    delete dcPtr; 
    outputFile.close();
}

/**
 * Metodo para realizar la convolucion de una Mascara con la animacion representada
 * en el EVM y almacenada en archivos de Couplets. 
 * @param mask: Mascara para realizar la convolucion.
 * @param endFrame: Maximo frame a procesar, el minimo es 0
 * @param _xMax: El maximo valor en X o ancho de los frames
 * @param _yMax: El maximo valor en Y o largo de los frames
 */
template<typename valueType>
void nDEVM<valueType>::maskAnimConv2(nDEVM * mask,int endFrame,valueType _xMax,
        valueType _yMax){    
    nDEVM<valueType> *currentResult;// = new nDEVM<valueType>();
    nDEVM<valueType> *sectionSeq = new nDEVM<valueType>();
    
    string fileName = "";
    string partName = "";
    double *dcPtr = new double;
    unsigned int i = 0; // - Mask Counter
    unsigned int dcFile = 0; // - Contador de archivos de DC
    unsigned int dcPart = mask->getCoord();

    // - Validar cuando se llega al final de la animacion
    if(mask->maskMax[0] > (endFrame + 1)){
        cout<<"Se ha llegado al final de la animacion..."<<endl;
        return;
    }
    
    sectionSeq = sectionSeq->maskAnimSections(mask,endFrame+1);
    
    // - Se guardan las rutas de los archivos correspondientes a esta parte
    // ***
    fileName = "..\\dcFiles\\Part"+to_string(dcPart)+"\\dcFiles.txt";
    
    ofstream dcFiles( fileName );
    if ( ! dcFiles.is_open() ){    
        cout << "El archivo "<<fileName<<" no se pudo abrir!" << '\n';    
        return;
    }

    // ***
    partName = "..\\dcFiles\\Part"+to_string(dcPart)+"\\dcFile"+to_string(dcFile)+".dc";
    ofstream outputFile( partName.c_str(),ios_base::out|ios_base::binary );
    if ( ! outputFile.is_open() ){    
        cout << "El archivo: "+partName+" no se pudo abrir!!" << '\n';    
        return;
    }
    
    dcFiles<<partName<<'\n';
    cout<<"Computing Part"<<dcPart<<"..."<<endl;
    // - Recorrido en y
    while(mask->maskMax[2] <= _yMax){
        // - Recorrido en x
        while(mask->maskMax[1] <= _xMax){
            currentResult = maskIntersection(mask,sectionSeq);

            *dcPtr = currentResult->discreteCompactness(mask->LcMin,mask->LcMax);
            delete currentResult;
//            cout<<"subSeq["<<i<<"] => DC: "<<*dcPtr<<endl;
            outputFile.write((char *) dcPtr,sizeof(double));

            mask->EVMTraslation(1,1); // - Desplazamiento en X
            i++;
            // - Los archivos de DC contienen un maximo de 5000 
            if(i >= 10000){
                outputFile.close();
                dcFile++;
                cout<<partName<<" ... DONE!!"<<endl;
                // ***
                partName = "..\\dcFiles\\Part"+to_string(dcPart)+"\\dcFile"+to_string(dcFile)+".dc";
                outputFile.open( partName.c_str(),ios_base::out|ios_base::binary );
                if ( ! outputFile.is_open() ){    
                    cout << "El archivo: "+partName+" no se pudo abrir!!" << '\n';    
                    return;
                }
                dcFiles<<partName<<'\n';
                i = 0;
            }
        }
        mask->maskDimReset(1);
        mask->EVMTraslation(2,1); // - Desplazamiento en Y
    }
    outputFile.close();
    dcFiles.close();
    mask->maskDimReset(1);
    mask->maskDimReset(2);

//    delete currentResult;
    delete dcPtr; 
    delete sectionSeq;
}

template<typename valueType>
void nDEVM<valueType>::dcContent(int _part,int _dc){
    // ***
    string fileName = "dcFiles/Part"+to_string(_part)+"/dcFile"+to_string(_dc)+".dc";
    ifstream fileInput;
    
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
    if (! fileInput.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    double *idx = new double;

    unsigned int i = 0;
    
    cout<<fileName<<" Content:"<<endl;
    while(fileInput.read((char *) idx, sizeof(double))){
        cout <<"Seq["<<i<<"]: "<<*idx <<endl; 
        i++;
    }
    fileInput.close();
    delete idx;
}

template<typename valueType>
valueType nDEVM<valueType>::content(){
    int dim = dimDepth();
    nDEVM<valueType> *p = this;
    return content(&p, dim);
}

template<typename valueType>
valueType nDEVM<valueType>::content(nDEVM **p, int n){
    valueType cont = 0, coordC1,coordC2;
    nDEVM<valueType> *couplet;
    nDEVM<valueType> *currentSection,*prevSection;
    
    if( (*p)->isEmpty() ){
        return 0;
    }
    
    if(n == 1){
        return (*p)->length();
    }
    
    prevSection = new nDEVM<valueType>();
    
    coordC1 = (*p)->getCoord();
    couplet = (*p)->readCouplet();
    
    while( !((*p)->endEVM()) ){
        coordC2 = (*p)->getCoord();
        
        currentSection = getSection(prevSection,couplet);

        cont = cont + content(&currentSection,n-1)*(coordC2 - coordC1);
        
        coordC1 = coordC2;
        delete couplet;
        couplet = (*p)->readCouplet();

        delete prevSection;
        prevSection = currentSection;
    }
    delete currentSection;
    delete couplet;
//    currentSection = NULL;
//    couplet = NULL;
//    currentCouplet = NULL;
    
    (*p)->resetCoupletIndex();
    return cont;
}

/**
 * Longitud de un 1D OPP...
 * @return 
 */
template<typename valueType>
valueType nDEVM<valueType>::length(){
    return trieTree->length();
}

template<typename valueType>
nDEVM<valueType> * nDEVM<valueType>::dimLeftShift(){
    nDEVM<valueType> * newEVM = new nDEVM<valueType>(trieTree->dimLeftShift());
    return newEVM;
}

template<typename valueType>
valueType nDEVM<valueType>::internalContacts(nDEVM * p,int n){
    nDEVM<valueType> * couplet;
    nDEVM<valueType> * prevSection,*currentSection;
    nDEVM<valueType> * sectionInt;
    valueType c1Coord,c2Coord,nCoords;
    valueType iContacts = 0;
    
    prevSection = new nDEVM<valueType>();
    c1Coord = p->getCoord();
    couplet = p->readCouplet();
    while( !(p->endEVM()) ){
        currentSection = getSection(prevSection,couplet);
        
        c2Coord = p->getCoord();
        nCoords = c2Coord - c1Coord - 1;
        
        iContacts = iContacts + nCoords * content(&currentSection,n-1);
        
        sectionInt = prevSection->booleanOperation(currentSection,"intersection",n-1);
        iContacts = iContacts + content(&sectionInt,n-1);
        
        delete sectionInt;
        delete prevSection;
        delete couplet;
        
        prevSection = currentSection;
        
        c1Coord = c2Coord;
        couplet = p->readCouplet();
    }
    p->resetCoupletIndex();
    delete currentSection;
    delete couplet;
    
    return iContacts;
}    

template<typename valueType>
valueType nDEVM<valueType>::totalInternalContacts(){
    int dim = dimDepth();
    valueType Lc = 0;
    nDEVM<valueType> *p;
    nDEVM<valueType> *prevP;
    
    Lc = Lc + internalContacts(this,dim);
    
    p = dimLeftShift();

    for(int i = 0; i < (dim-1); i++){
        Lc = Lc + internalContacts(p,dim);
        prevP = p;
        p = p->dimLeftShift();
        
        delete prevP;
    }
    delete p;
    return Lc;
}

template<typename valueType>
double nDEVM<valueType>::discreteCompactness(valueType lMin,valueType lMax){
    valueType Lc = totalInternalContacts();
    
    return ((double)(Lc - lMin))/((double)(lMax - lMin));
}

//template<typename valueType>
//void nDEVM<valueType>::subAnimClustering(int clusters,int _parts,int _dcFiles){
////    string fileName = "dcFiles/dcFile_22_04_2015.dc";    
//    som = new SOM(clusters,_parts,_dcFiles);
////    som->loadBinFile(fileName);
//    som->initialize();
//    som->sampling();
//    som->clustering();    
//    delete som;
//}

template<typename valueType>
void nDEVM<valueType>::subAnimClustering(int clusters,int _dcParts,int _iter){
//    string fileName = "dcFiles/dcFile_22_04_2015.dc";    
    som = new SOM(clusters,_dcParts,_iter);
//    som->loadBinFile(fileName);
    som->initialize();
    som->sampling();
    som->clustering();    
    delete som;
}

template<typename valueType>
void nDEVM<valueType>::clusterContent(int cluster){
    // ***
    string fileName = "clustering/cluster"+to_string(cluster)+".idx";
    ifstream fileInput;
    
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
    if (! fileInput.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    unsigned int *idx = new unsigned int;

    int i = 0;
    
    cout<<"Cluster #"<<cluster<<" Content:"<<endl;
    while(fileInput.read((char *) idx, sizeof(unsigned int))){
        cout <<"Seq["<<i<<"]: "<<*idx <<endl; 
        i++;
    }
    fileInput.close();
    delete idx;
    
}

template<typename valueType>
void nDEVM<valueType>::clusterContent(int cluster, nDEVM * mask,int initFrame,int endFrame,
        valueType _xMax,valueType _yMax){
    valueType xCount = 119, xShift;
    valueType yCount = 79, yShift;
    valueType totalCount = xCount * yCount, tShift;
    valueType steps = 2;

    nDEVM<valueType> *prevResult;// = new nDEVM<valueType>();
    nDEVM<valueType> *currentResult = new nDEVM<valueType>();
    nDEVM<valueType> *finalResult = new nDEVM<valueType>();
    
    // - Lectura del archivo de DC
    string fileName = "clustering/cluster"+to_string(cluster)+".idx";
    ifstream fileInput;
    
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
    if (! fileInput.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    unsigned int *idx = new unsigned int; // para leer el contenido del dcFile
    unsigned int i = 0; // Numero de secuencia...
    
    cout<<"Cluster #"<<cluster<<" Content:"<<endl;
    // - Mientras haya informacion en el archivo...
    while(fileInput.read((char *) idx, sizeof(unsigned int))){
        tShift = (unsigned int) (*idx)/totalCount;
        yShift = (unsigned int) ( (*idx) - (tShift * totalCount) )/xCount;
        xShift = (*idx) - (tShift * totalCount) - yShift*xCount;
        cout <<"Seq["<<i<<"]: "<<*idx<<", tShift: "<<tShift<<", yShift: "<<yShift
                <<", xShift: "<<xShift<<endl; 

        mask->EVMTraslation(0,tShift);
        mask->EVMTraslation(1,xShift*steps);
        mask->EVMTraslation(2,yShift*steps);
        
        prevResult = finalResult;
        
        currentResult = maskIntersection(mask,initFrame,endFrame+1);
        finalResult = finalResult->booleanOperation(currentResult,"union");
        
        delete currentResult;
        delete prevResult;
        mask->maskDimReset(0);
        mask->maskDimReset(1);
        mask->maskDimReset(2);
        i++;
    }
    
    i = 0;
    nDEVM<unsigned int> *couplet;
    nDEVM<unsigned int> *currentSection,*prevSection;
    currentSection= new nDEVM<unsigned int>();
    
    while(!finalResult->endEVM()){
        couplet = finalResult->readCouplet();
        couplet->EVMFile("clustering/Cluster"+to_string(cluster),"clusterCouplet",i);
        prevSection = currentSection;
        currentSection = getSection(prevSection,couplet);        
        currentSection->EVMFile("clustering/Cluster"+to_string(cluster),"clusterSection",i);
        
        delete prevSection;
        delete couplet;
        i++;
    }
    delete currentSection;

    finalResult->resetCoupletIndex();

    fileInput.close();
    delete idx;
}

template<typename valueType>
void nDEVM<valueType>::clusterContent2(int cluster, nDEVM * mask,int endFrame,
        valueType _width,valueType _length){
    unsigned int _xLength = mask->dimMax(1);
    unsigned int _yLength = mask->dimMax(2);
    
    valueType xCount = _width - _xLength + 1 , xShift;
    valueType yCount = _length - _yLength + 1, yShift;
    valueType totalCount = xCount * yCount, tShift;
    valueType steps = 1;

    nDEVM<valueType> *prevResult;// = new nDEVM<valueType>();
    nDEVM<valueType> *currentResult;
    nDEVM<valueType> *finalResult = new nDEVM<valueType>();
    nDEVM<unsigned int> *sectionSeq;
    
    sectionSeq = maskAnimSections(mask,endFrame+1);
    
    // - Lectura del archivo de DC
    // *** CAMBIAR PARA SCRIPTS EJECUTADOS EN CONSOLA
//    string fileName = "clustering/cluster"+to_string(cluster)+".idx";
    string fileName = "..\\clustering\\cluster"+to_string(cluster)+".idx";
    ifstream fileInput;
    
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
    if (! fileInput.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    unsigned int *idx = new unsigned int; // para leer el contenido del dcFile
    unsigned int i = 0; // Numero de secuencia...
    
    cout<<"Getting cluster's #"<<cluster<<" Content:"<<endl;
    // - Mientras haya informacion en el archivo...
    while(fileInput.read((char *) idx, sizeof(unsigned int))){
        tShift = (unsigned int) (*idx)/totalCount;
        yShift = (unsigned int) ( (*idx) - (tShift * totalCount) )/xCount;
        xShift = (*idx) - (tShift * totalCount) - yShift*xCount;
//        cout <<"Seq["<<i<<"]: "<<*idx<<", tShift: "<<tShift<<", yShift: "<<yShift
//                <<", xShift: "<<xShift<<endl; 

        mask->EVMTraslation(0,tShift);
        mask->EVMTraslation(1,xShift*steps);
        mask->EVMTraslation(2,yShift*steps);
        
        // - Estraer la secuencia de Secciones, solo si el EVM de la secuencia esta vacio
        // o el desplazamiento en el tiempo no coincide con la mascara
        if(sectionSeq->getCoord() != mask->getCoord()){
            cout <<"Seq["<<i<<"]: "<<*idx<<", tShift: "<<tShift<<", yShift: "<<yShift
                    <<", xShift: "<<xShift<<endl;                         
            delete sectionSeq;
            sectionSeq = maskAnimSections(mask,endFrame+1);
        }
        
        prevResult = finalResult;
        
        currentResult = maskIntersection(mask,sectionSeq);
        finalResult = finalResult->booleanOperation(currentResult,"union");
        
        delete currentResult;
        delete prevResult;
        mask->maskDimReset(0);
        mask->maskDimReset(1);
        mask->maskDimReset(2);
        i++;
    }
    fileInput.close();

    cout<<"Getting images..."<<endl;
    // *** CAMBIAR PARA SCRIPTS EJECUTADOS EN CONSOLA
    finalResult->animImages(_width,_length,"..\\clustering\\Cluster"+
            to_string(cluster)+"\\clusterSection");
//    finalResult->animImages(_width,_length,"clustering/Cluster"+
//            to_string(cluster)+"/clusterSection");
    
//    i = 0;
//    nDEVM<unsigned int> *couplet;
//    nDEVM<unsigned int> *currentSection,*prevSection;
//    currentSection= new nDEVM<unsigned int>();
//    
//    cout<<"Getting cluster images..."<<endl;
//    while(!finalResult->endEVM()){
//        couplet = finalResult->readCouplet();
////        couplet->EVMFile("clustering/Cluster"+to_string(cluster),"clusterCouplet",i);
//        prevSection = currentSection;
//        currentSection = getSection(prevSection,couplet);        
////        currentSection->EVMFile("clustering/Cluster"+to_string(cluster),"clusterSection",i);
//        // *** CAMBIAR PARA SCRIPTS EJECUTADOS EN CONSOLA
//        currentSection->frameToImage(_width,_length,1,
//                "..\\clustering\\Cluster"+to_string(cluster)+"\\clusterSection"+to_string(i));
//        delete prevSection;
//        delete couplet;
//        i++;
//    }
//    delete currentSection;

//    finalResult->resetCoupletIndex();
    delete finalResult;
    delete idx;
}

template<typename valueType>
void nDEVM<valueType>::clusterContent3(int cluster, nDEVM * mask,int endFrame,
        valueType _width,valueType _length){
    unsigned int _xLength = mask->dimMax(1);
    unsigned int _yLength = mask->dimMax(2);
    
    valueType xCount = _width - _xLength + 1 , xShift;
    valueType yCount = _length - _yLength + 1, yShift;
    valueType totalCount = xCount * yCount, tShift;
    valueType steps = 1;

    nDEVM<valueType> *prevResult;// = new nDEVM<valueType>();
    nDEVM<valueType> *currentResult;
    nDEVM<valueType> *finalResult = new nDEVM<valueType>();
    nDEVM<valueType> *prevMask;
    nDEVM<valueType> *superMask = new nDEVM<valueType>();
    vector<string> evmVector; // - Nombres de archivos EVM de las partes del cluster
    nDEVM<unsigned int> *sectionSeq;
    
    sectionSeq = maskAnimSections(mask,endFrame+1);
    
    // - Lectura del archivo de DC
    // *** CAMBIAR PARA SCRIPTS EJECUTADOS EN CONSOLA
//    string fileName = "clustering/cluster"+to_string(cluster)+".idx";
    string fileName = "..\\clustering\\cluster"+to_string(cluster)+".idx";
//    string partName = "clustering/Cluster"+to_string(cluster)+"/clusterPart";
    // - Partes en que se divide el cluster, contiene las intersecciones con las mascaras y sus
    // desplazamientos...
    string partName = "..\\clustering\\Cluster"+to_string(cluster)+"\\clusterPart";

    ifstream fileInput;    
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
    if (! fileInput.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    unsigned int *idx = new unsigned int; // para leer el contenido del dcFile
    unsigned int i = 0,clusterPart = 0; // Numero de secuencia...
    
//    cout<<"Getting cluster's #"<<cluster<<" Content:"<<endl;
    // - Mientras haya informacion en el archivo...
    while(fileInput.read((char *) idx, sizeof(unsigned int))){
        tShift = (unsigned int) (*idx)/totalCount;
        yShift = (unsigned int) ( (*idx) - (tShift * totalCount) )/xCount;
        xShift = (*idx) - (tShift * totalCount) - yShift*xCount;

        mask->EVMTraslation(0,tShift);
        mask->EVMTraslation(1,xShift*steps);
        mask->EVMTraslation(2,yShift*steps);
        
        prevMask = superMask;
        superMask = superMask->booleanOperation(mask,"union");
        delete prevMask;
        
        // - Estraer la secuencia de Secciones, solo si el EVM de la secuencia esta vacio
        // o el desplazamiento en el tiempo no coincide con la mascara
        if(sectionSeq->getCoord() != mask->getCoord()){
//            cout <<"Seq["<<i<<"]: "<<*idx<<", tShift: "<<tShift<<", yShift: "<<yShift
//                    <<", xShift: "<<xShift<<endl;                         

            if(!superMask->isEmpty()){
                finalResult = maskIntersection(superMask,sectionSeq);;
                // - Guardar archivo binario del resultado actual...
                cout<<"Saving: "<<partName+to_string(clusterPart)+".evm"<<endl;
                finalResult->saveEVM2(partName,clusterPart);
                evmVector.push_back(partName+to_string(clusterPart));
                clusterPart++;
            }

            delete finalResult;
            finalResult = new nDEVM<valueType>();
            delete sectionSeq;
            sectionSeq = maskAnimSections(mask,endFrame+1);
            delete superMask;
            superMask = new nDEVM<valueType>();
        }
        
        mask->maskDimReset(0);
        mask->maskDimReset(1);
        mask->maskDimReset(2);
        i++;
    }
    fileInput.close();
    
    if(!superMask->isEmpty()){
        finalResult = maskIntersection(superMask,sectionSeq);;
        // - Guardar archivo binario del resultado actual...
        cout<<"Saving: "<<partName+to_string(clusterPart)+".evm"<<endl;
        finalResult->saveEVM2(partName,clusterPart);
        evmVector.push_back(partName+to_string(clusterPart));
        clusterPart++;
    }
    delete finalResult;
    delete sectionSeq;
    delete superMask;

    cout<<"Computing EVM vector Union..."<<endl;
    // - Archivo con el nombre de las Secciones del EVM
    fileName = "..\\clustering\\Cluster"+to_string(cluster)+"\\clusterSections.txt";
    
    ofstream clusterSectionsFile( fileName );
    if ( ! clusterSectionsFile.is_open() ){    
        cout << "El archivo "<<fileName<<" no se pudo abrir!" << '\n';    
        return;
    }
    
    unsigned int vectorSize = evmVector.size();

    finalResult = new nDEVM<valueType>();

    for(unsigned int i = 0; i < vectorSize; i++){
        currentResult = new nDEVM<valueType>();
        currentResult->readEVM2(evmVector[i]);

        prevResult = finalResult;
        finalResult = currentResult->booleanOperation(prevResult,"union");

        delete currentResult;
        delete prevResult;
    }
    
    i = 0;
    nDEVM<unsigned int> *couplet;
    nDEVM<unsigned int> *currentSection,*prevSection;
    currentSection= new nDEVM<unsigned int>();
    
    while(true){
        couplet = finalResult->readCouplet();
        
        if(finalResult->endEVM()){
            break;
        }
        prevSection = currentSection;
        currentSection = getSection(prevSection,couplet);
        currentSection->saveEVM2("..\\clustering\\Cluster"+to_string(cluster)+"\\clusterSection",i);
        clusterSectionsFile<<"..\\clustering\\Cluster"+to_string(cluster)+
                "\\clusterSection" +to_string(i)<<endl;
        delete prevSection;
        delete couplet;
        i++;
    }
    delete currentSection;    
    clusterSectionsFile.close();
    
//    cout<<"Getting images..."<<endl;
//    // *** CAMBIAR PARA SCRIPTS EJECUTADOS EN CONSOLA
//    finalResult->animImages(_width,_length,"..\\clustering\\Cluster"+
//            to_string(cluster)+"\\clusterSection");
////    finalResult->animImages(_width,_length,"clustering/Cluster"+
////            to_string(cluster)+"/clusterSection");
    
    delete finalResult;
    delete idx;
}

template<typename valueType>
void nDEVM<valueType>::clusterFrame(int cluster, string fileName,int idx,valueType _width,
        valueType _length){
    string imageName = "..\\clustering\\Cluster"+to_string(cluster)+"\\clusterSection"
            +to_string(idx);
    nDEVM<unsigned int> *section = new nDEVM<unsigned int>();
    section->readEVM2(fileName);
//    cout<<"Saving "<<imageName<<".bmp"<<endl;
    if(section->dimDepth() > 0)
        section->frameToImage(_width,_length,section->dimDepth() - 2,imageName);
    delete section;
}

template<typename valueType>
void nDEVM<valueType>::animImages(valueType _width, valueType _length,
        string imageName){
    unsigned int i = 0;
    nDEVM<unsigned int> *couplet;
    nDEVM<unsigned int> *currentSection,*prevSection;
    currentSection= new nDEVM<unsigned int>();
    
    while(!endEVM()){
        couplet = readCouplet();
//        couplet->EVMFile("clustering/Cluster"+to_string(cluster),"clusterCouplet",i);
        prevSection = currentSection;
        currentSection = getSection(prevSection,couplet);        
//        currentSection->EVMFile("clustering/Cluster"+to_string(cluster),"clusterSection",i);
        currentSection->frameToImage(_width,_length,1,imageName+to_string(i));
        delete prevSection;
        delete couplet;
        i++;
    }
    delete currentSection;    
}

template<typename valueType>
void nDEVM<valueType>::dcNormalization(int _parts,int _dcFiles){
    string fileName = "";
    string part = "";
    double *dcValue = new double;
    double min = 1.0, max = 0.0;
    
    for(int j = 0; j < _parts; j++){
        part = "Part"+to_string(j);
        // - Lectura de los archivos de cada parte
        for(int k = 0; k < _dcFiles; k++){
            fileName = "dcFiles/"+part+"/dcFile"+to_string(k)+".dc";
            cout<<"Reading File: "<<fileName<<endl;
            ifstream fileInput;
            fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file

            if (! fileInput.is_open()){
                cout<<"El archivo: "<<fileName<<" no pudo abrirse!!!..."<<endl;
                return;
            }

            while(fileInput.read((char *) dcValue, sizeof(double))){
                if((*dcValue) < min){
                    min = *dcValue;
                }
                if((*dcValue) > max){
                    max = *dcValue;
                }
            }
            fileInput.close();
        }
    }
    
    cout<<"maxDC: "<<max<<", minDC: "<<min<<endl;
    dcNormalization(_parts,_dcFiles,max,min);
}

template<typename valueType>
void nDEVM<valueType>::dcNormalization(int _parts,int _dcFiles,double max,double min){
    string fileName = "";
    string fileNorm = "";
    string part = "";
    double *dcValue = new double;
    
    for(int j = 0; j < _parts; j++){
        part = "Part"+to_string(j);
        // - Lectura de los archivos de cada parte
        for(int k = 0; k < _dcFiles; k++){

            fileName = "dcFiles/"+part+"/dcFile"+to_string(k)+".dc";
            fileNorm = "dcFiles/"+part+"/dcNFile"+to_string(k)+".dc";
            ifstream fileInput;
            fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file

            if (! fileInput.is_open()){
                cout<<"El archivo: "<<fileName<<" no pudo abrirse!!!..."<<endl;
                return;
            }
//                {
//                    cout<<"Reading File: "<<fileName<<endl;
//                }
            ofstream outputFile( fileNorm.c_str(),ios_base::out|ios_base::binary );
            
            if ( ! outputFile.is_open() ){    
                cout << "El archivo: "+fileName+" no se pudo abrir!!" << '\n';    
                return;
            }

            while(fileInput.read((char *) dcValue, sizeof(double))){
//                cout<<"DCValue: "<<*dcValue;
                *dcValue = (*dcValue-min)/(max-min);
//                cout<<", DCNValue: "<<*dcValue<<endl;
                outputFile.write((char *) dcValue,sizeof(double));
            }
            outputFile.close();
            fileInput.close();

        }
    }
    
}

template<typename valueType>
void nDEVM<valueType>::frameMaskInit(int colorComponents, int colorCompSize){
    maskMin[0] = 0;
    maskMax[0] = 1;
    maskMin[1] = 0;
    maskMax[1] = 1;
    
    valueType colorCompMax = pow(2,colorCompSize*8);
    valueType * maskVoxel = new valueType[2+colorComponents];
    valueType * maskLengths = new valueType[2+colorComponents];
    
    maskVoxel[0] = 0;
    maskLengths[0] = 1;
    
    maskVoxel[1] = 0;
    maskLengths[1] = 1;
    
    for(int i = 0; i < colorComponents; i++){
        maskVoxel[2+i] = 0;
        maskLengths[2+i] = colorCompMax;
    }
    populateMask(&maskVoxel,2+colorComponents,0,&maskLengths);
    
    delete [] maskVoxel;
    delete [] maskLengths;
}

template<typename valueType>
valueType nDEVM<valueType>::dimMax(int dim){
    return trieTree->dimMax(dim);
}

template<typename valueType>
void nDEVM<valueType>::frameToImage(int width, int height,int colorCount,string imageName){
    BMP *bmp = new BMP(width, height,colorCount);
    bmp->pImageData = new BYTE[width*height*colorCount];

    nDEVM<unsigned int> *frameMask = new nDEVM<unsigned int>();
    
    frameMask->frameMaskInit(colorCount,1);
//    frameMask->printEVM();
//    cout<<"FrameMask Dim: "<<frameMask->dimDepth()<<endl;
    
    nDEVM<unsigned int> *inter;

//    cout<<"Retrieving pixel info..."<<endl;
    // - Maximo en x
    int j = 0;
    while(frameMask->maskMax[1] <= height){
//        cout<<"Processing line "<<frameMask->maskMax[1]<<endl;
        while(frameMask->maskMax[0] <= width){
            inter = booleanOperation(frameMask,"intersection");
            
            for(int i = 0; i < colorCount; i++){
                bmp->pImageData[j] = inter->dimMax(2+i) - 1;
                j++;
            }
            
            frameMask->EVMTraslation(0,1);
            delete inter;
        }
        frameMask->maskDimReset(0);
        frameMask->EVMTraslation(1,1);
    }
    // CAMBIAR PARA SCRIPTS EJECUTADOS EN CONSOLA
    cout<<"Saving: "<<imageName+".bmp"<<endl;
    bmp->saveImage(imageName+".bmp");    
//    delete bmp;
}
