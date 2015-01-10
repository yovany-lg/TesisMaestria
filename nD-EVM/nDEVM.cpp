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
#include <cmath>

#include "nDEVM.h"

template <typename T> std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

nDEVM::nDEVM() {
//    rootNode = NULL;
//    resetCoupletIndex();
    trieTree = new TrieTree();
}

nDEVM::nDEVM(TrieTree *trie) {
    trieTree = trie;
//    rootNode = node;
//    resetCoupletIndex();
}

nDEVM::nDEVM(const nDEVM& orig) {
}

nDEVM::~nDEVM() {   //METODO PARA ELIMINAR EL ARBOL TRIE
    delete trieTree;
    //trieTree->deleteTrie();
}

void nDEVM::resetCoupletIndex(){
    trieTree->resetCoupletIndex();
}

bool nDEVM::isEmpty(){
    trieTree->isEmpty();
}


/*Método Auxiliar para Insertar un vértice en un árbol trie.
 * Argumentos:
 * -Nodo raíz.
 * -Apuntador al vector de entrada
 * -Tamaño del vector.
 */
void nDEVM::insertVertex(double * inputKey,int length){
    trieTree->insertVertex(inputKey,length);
}

/**
 * Método Auxiliar para Imprimir en consola el contenido de un árbol Trie.
 */
void nDEVM::printEVM(){
    trieTree->printTrie();
}

/*Método Auxiliar para obtener el Tamaño de un árbol trie.
 * Argumentos:
 * -Nodo raíz.
*/
double nDEVM::size(){
    return trieTree->size();
}

/**
 * Método para comparar dos EVMs.
 * @param otherEVM: El EVM que será comparado con el EVM actual.
 * @return Se retorna un valor booleano (true, false).
 */
bool nDEVM::compareEVM(nDEVM *otherEVM){
    return trieTree->compare(otherEVM->trieTree);
}

/**
 * Método para comparar dos EVMs considerando los couplets sobre la primera dimensión
 * @param otherEVM: El EVM que será comparado con el EVM actual.
 * @return Se retorna un valor booleano (true, false).
 */
bool nDEVM::compareByCouplets(nDEVM *otherEVM){
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
nDEVM * nDEVM::cloneEVM(){
    TrieTree *copyTrie = trieTree->clone();
    nDEVM *cloneEVM = new nDEVM(copyTrie);
    return cloneEVM;
}

/*Método Auxiliar para remover un vertice de un árbol trie.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Apuntador al vector del elemento a eliminar.
*/
void nDEVM::removeVertex(double *key){
    trieTree->removeVertex(key);
}

/*Método Auxiliar para verificar que un vertice exista en un árbol trie.
 * Argumentos:
 * -Nodo raíz del arbol.
 * -Apuntador al vector del vertice.
 * -Tamaño del vector.
*/
bool nDEVM::existsVertex(double * inputKey,int length){
    return trieTree->existsVertex(inputKey,length);
}

/**
 * Método para realizar la operación XOR de dos EVMs.
 * @param otherEVM: El otro EVM.
 * @return El un nuevo EVM obtenido al operar los dos EVMs.
 */
nDEVM* nDEVM::mergeXOR(nDEVM* otherEVM){
    //Se considera que esta operacion se realizara con EVMs de la misma dimension.
    if(isEmpty() and !(otherEVM->isEmpty()))
        return otherEVM->cloneEVM();
   
    if(!isEmpty() and otherEVM->isEmpty())
        return cloneEVM();

    if(isEmpty() and otherEVM->isEmpty()){
        nDEVM *xorEVM = new nDEVM();
        return xorEVM;
    }
    
    TrieTree *resultTrie = trieTree->XOR(otherEVM->trieTree);
    nDEVM *resultEVM = new nDEVM(resultTrie);
    return resultEVM;
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
    
    cout << "Se finalizo la lectura del archivo: "<< fileName << "\n";
    delete newKey;
}

/**
 * Cargar un archivo binario, que representa una voxelizacion en nD, en el EVM...
 * @param fileName: Nombre del archivo...
 * @param voxelSize: Tamaño de la voxelización...
 * @param dim: Dimensión del espacio.
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
    
    delete newVoxel;
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

/*Método para generar e insertar la voxelización que consiste de 8 vértices, generada a partir del vértice en el origen.
 * Argumentos:
 * -Nodo Raiz.
 * -Apuntador doble del vector de entrada.
*/
void nDEVM::populate3DVoxel(double **voxelInput){
    populateVoxel(voxelInput,3,0,1);
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
    EVMFile("",index);
}

void nDEVM::EVMFile(string suffix, int index){
    double * testKey = new double[4];   //Eliminar
    string fileName = "EVMFiles/EVMFile_"+suffix+to_string(index)+".evm";
    
    ofstream outputFile( fileName );
    if ( ! outputFile.is_open() ){    
        cout << "El archivo no se pudo abrir!" << '\n';    
        return;
    } 
    outputFile<<"XYZ"<<'\n'<<'3'<<'\n';
    EVMFile(&outputFile,trieTree->rootNode,&testKey,0);
    outputFile.close();
    delete testKey;
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
    trieTree->putCouplet2(couplet->trieTree);
//    putCouplet(&prevNode,&rootNode,couplet->rootNode);
}

/*
 *Metodo auxiliar para agregar una Seccion perpendicular a la dimension x1:
 * Argumentos:
 * -El (n-1)D EVM del couplet.
 */
void nDEVM::putSection(nDEVM * section){
    trieTree->putCouplet2(section->trieTree);
}

/**
 * Obtiene el couplet de la primera dimension asociado a la posicion donde
 * actualmente se encuentra el apuntador de couplets coupletIndex.
 * @return 
 * nDEVM*
 */
nDEVM* nDEVM::readCouplet(){
    //SI SE ELIMINA EL EVM RETORNADO, SE ELIMINARA DEL EVM ORIGINAL
    nDEVM *coupletEVM = new nDEVM(trieTree->readCouplet());
    return coupletEVM;
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
    return trieTree->endTrie();
}

/**
 * Metodo que inserta un nuevo nodo en la dimension x1 con el valor dado, y 
 * el nuevo nodo sera el nodo raiz.
 * @param coord
 * @return 
 */
void nDEVM::setCoord(double coord){
    trieTree->setCoord(coord);
}

/**
 * Metodo que devuelve el valor de la primera dimension del couplet asociado al
 * nodo raiz.
 * @return 
 */
double nDEVM::getCoord(){
    return trieTree->getCoord();
}

/**
 * Devuelve la operación mergeXOR entre una sección y un couplet, con lo que se
 * obtiene la siguiente sección...
 * @param section
 * @param couplet
 * @return Un nuevo EVM resultante.
 */
nDEVM* nDEVM::getSection(nDEVM* section, nDEVM *couplet){
    return section->mergeXOR(couplet);
}

/**
 * Método que realiza la operación mergeXOR entre dos secciones, con lo que se
 * se obtiene el couplet que se encuentra entre ambas secciones...
 * @param section1
 * @param section2
 * @return Un nuevo EVM resultante.
 */
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
        
//        currentSection->EVMFile("Section",i);
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
int nDEVM::dimDepth(){
    return trieTree->dimDepth();
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
    nDEVM *pSection,*pPrevSection, *qSection,*qPrevSection, *couplet;
    nDEVM *result, *rPrevSection, *rCurrentSection;
    bool fromP, fromQ;
    double coord;
    if(n == 1){
        return booleanOperation(p,q,op);
    }
    pSection = new nDEVM();
    qSection = new nDEVM();
    rCurrentSection = new nDEVM();
    result = new nDEVM();
    while(!(p->endEVM()) and !(q->endEVM())){
        nextObject(p,q,&coord,&fromP,&fromQ);
        if(fromP){
            couplet = p->readCouplet();
            pPrevSection = pSection;
            pSection = getSection(pSection,couplet);
            delete pPrevSection;    //Liberar Memoria

//            cout<<"section fromP, coord: "<<coord<<"\n";
//            pSection->printTrie();
        }
        if(fromQ){
            couplet = q->readCouplet();
            qPrevSection = qSection;
            qSection = getSection(qSection,couplet);
            delete qPrevSection;    //Liberar Memoria

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
            couplet->setCoord(coord);

            result->putCouplet(couplet);
        }
        
        delete rPrevSection;    //Liberar Memoria
    }
    while(!(p->endEVM())){
//        cout<<"I was Here...\n";
        if(putCoupletByOp(op,1)){
            coord = (*(p->trieTree->coupletIndex))->value;
            couplet = (p->readCouplet())->cloneEVM();
            couplet->setCoord(coord);
            result->putCouplet(couplet);
        }else
            break;
    }

    while(!(q->endEVM())){
//        cout<<"I was Here...\n";
        if(putCoupletByOp(op,2)){
            coord = (*(q->trieTree->coupletIndex))->value;
            couplet = (q->readCouplet())->cloneEVM();
            couplet->setCoord(coord);
            result->putCouplet(couplet);
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

void nDEVM::nextObject(nDEVM *p, nDEVM *q,double *coord,bool *fromP, bool *fromQ){
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

nDEVM* nDEVM::booleanOperation(nDEVM *section1, nDEVM *section2, string op){
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
}

bool nDEVM::putCoupletByOp(string op,int argPosition){
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
    
    TrieTree *resultTrie = new TrieTree();
    resultTrie = resultTrie->unionOperation(section1->trieTree,section2->trieTree);
    nDEVM *resultEVM = new nDEVM(resultTrie);
    return resultEVM;
}

nDEVM* nDEVM::intersectionOperation(nDEVM* section1, nDEVM* section2){
    // Casos en el que uno o ambos EVMs estan vacios
    if(section1->isEmpty() or section2->isEmpty()){
        return new nDEVM();
    }
        
    TrieTree *resultTrie = new TrieTree();
    resultTrie = resultTrie->intersectionOperation(section1->trieTree,section2->trieTree);

    nDEVM *result = new nDEVM(resultTrie);
    
    return result;
}

nDEVM* nDEVM::differenceOperation(nDEVM* section1, nDEVM* section2){
    // A - B
    if(!section1->isEmpty() and section2->isEmpty()){
        return section1->cloneEVM();
    }
        
    if(section1->isEmpty() and section2->isEmpty()){
        return new nDEVM();
    }
    
    TrieTree *resultTrie = new TrieTree();
    resultTrie = resultTrie->differenceOperation(section1->trieTree,section2->trieTree);
    
    nDEVM *result = new nDEVM(resultTrie);
    
    return result;
}

nDEVM* nDEVM::xorOperation(nDEVM* section1, nDEVM* section2){
    // A xor B
    if(!section1->isEmpty() and section2->isEmpty()){
        return section1->cloneEVM();
    }
        
    if(section1->isEmpty() and !section2->isEmpty()){
        return section2->cloneEVM();
    }

    TrieTree *resultTrie = new TrieTree();
    resultTrie = resultTrie->xorOperation(section1->trieTree,section2->trieTree);
    
    nDEVM *result = new nDEVM(resultTrie);
    return result;
}

//double nDEVM::content(){
//    int dimDepth = getDimDepth();
//    return content(this, dimDepth);
//}
//
//double nDEVM::content(nDEVM *p, int n){
//    double cont = 0, coordC1,coordC2;
//    nDEVM *couplet1, *couplet2;
//    nDEVM *section;
//    
//    if(n == 1){
//        return p->length();
//    }
//    
//    couplet1 = new nDEVM();
//    couplet2 = new nDEVM();
//    section = new nDEVM();
//    
//    coordC1 = p->coupletCoord();
//    couplet1 = p->readCouplet();
//    
//    while( !(p->endEVM()) ){
//        coordC2 = p->coupletCoord();
//        couplet2 = p->readCouplet();
//        section = getSection(section,couplet1);
//        
//        cont = cont + content(section,n-1)*(coordC2 - coordC1);
//        coordC1 = coordC2;
//        couplet1 = couplet2;
//    }
//    p->resetCoupletIndex();
//    return cont;
//}
//
//double nDEVM::boundaryContent(){
//    int dimDepth = getDimDepth();
//    return boundaryContent(this,dimDepth);
//}
//
//double nDEVM::boundaryContent(nDEVM *p, int n){
//    double cont = 0, coordC1,coordC2;
//    nDEVM *couplet1, *couplet2;
//    nDEVM *section;
//    
//    if(n == 2){
//        // - Solo para 2DEVMs
//        return p->perimeter();
//    }
//    
//    couplet1 = new nDEVM();
//    couplet2 = new nDEVM();
//    section = new nDEVM();
//    
//    coordC1 = p->coupletCoord();
//    couplet1 = p->readCouplet();
//    
//    while( !(p->endEVM()) ){
//        coordC2 = p->coupletCoord();
//        couplet2 = p->readCouplet();
//        section = getSection(section,couplet1);
//        
//        cont = cont + content(couplet1,n-1) + boundaryContent(section,n-1)*(coordC2 - coordC1);
//        coordC1 = coordC2;
//        couplet1 = couplet2;
//    }
//    cont = cont + content(couplet1,n-1);
//    p->resetCoupletIndex();
//    return cont;
//}
//
//double nDEVM::length(){
//    double l = 0;
//    trieNode *segment;
//
//    if(isEmpty())
//        return 0;
//    
//    segment = rootNode;    
//    l += (segment->nextTrieNode->value - segment->value );
//    
//    while(segment->nextTrieNode->nextTrieNode != NULL){
//
//        segment = segment->nextTrieNode->nextTrieNode;    
//        l += (segment->nextTrieNode->value - segment->value );
//    }
//    return l;
//}
//
///**
// * Solo para 2DEVMs
// * @return 
// */
//double nDEVM::perimeter(){
//    double p = 0;
//    // - explorar el EVM de donde sellamo la funcion
//    nDEVM *couplet;
//    nDEVM *q = new nDEVM();
//    double *otherInputs = new double[2];
//    trieNode *segment;
//    while(!endEVM()){
//        // - coordenada en x1 ahora se pasa a la segunda dimensión en ordenamiento
//        otherInputs[1] = coupletCoord();
//        
//        // - obtener la longitud del couplet 1D ordenado eobre x2
//        couplet = readCouplet();
//        p += couplet->length();
//        
//        // - explorar el segmento del couplet actual, el cual esta ordenado sobre x2
//        segment = couplet->rootNode;
//        // - pasar los vertices de los segmentos como la coordenada x1
//        otherInputs[0] = segment->value;
//        q->insertVertex(otherInputs,2);
//        otherInputs[0] = segment->nextTrieNode->value;
//        q->insertVertex(otherInputs,2);
//        while( segment->nextTrieNode->nextTrieNode != NULL ){
//            // - pasar los vertices de los segmentos como la coordenada x1
//            segment = segment->nextTrieNode->nextTrieNode;
//            otherInputs[0] = segment->value;
//            q->insertVertex(otherInputs,2);
//            otherInputs[0] = segment->nextTrieNode->value;
//            q->insertVertex(otherInputs,2);
//        }
//    }
//    
//    // - obtener las longitudes de los couplets pero con la primerdomensión x2
//    while( !(q ->endEVM()) ){        
//        couplet = q->readCouplet();
//        p += couplet->length();
//    }    
//    return p;
//}
//
//double nDEVM::discreteCompactness(){
//    int EVMContent = content();
//    double minContactArea,maxContactArea,boundaryCont,contactArea, DC;
//    
//    minContactArea=(EVMContent-1);
//    maxContactArea=3*(EVMContent - pow( pow(EVMContent,(double)1/3) , 2));
//    
//    boundaryCont = boundaryContent();
//    
//    contactArea = (6*EVMContent - boundaryCont)/2;
//    DC = (contactArea - minContactArea)/(maxContactArea - minContactArea);
//    
//    cout<<"Content: "<<EVMContent<<", BoundaryContent: "<<boundaryCont<<", ContactArea: "<<contactArea<<", DiscreteCompactness: "<<DC<<"\n";
//    
//    return DC;
//}

void nDEVM::loadImageFile(string fileName){
    TrieTree *imageTrie = new TrieTree();
    
    imageTrie->loadImage(fileName);
//    int size = imageTrie->size();
    return;
}