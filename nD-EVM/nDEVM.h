/* 
 * File:   nDEVM.h
 * Author: yova
 *
 * Created on 12 de agosto de 2014, 03:29 PM
 */
#include <cstring>
#include <string>

#ifndef NDEVM_H
#define	NDEVM_H

struct trieNode{
    double value;  //Valor de la dimension actual
    trieNode *nextDim;
    trieNode *nextTrieNode;
};

using namespace std;

class nDEVM {
private:
    trieNode *rootNode;
    trieNode **coupletIndex;
public:
    nDEVM();
    nDEVM(trieNode *node);
    nDEVM(const nDEVM& orig);
    virtual ~nDEVM();
    /*Metodos del Trie*/
    trieNode *getRootNode();
    bool isEmpty();
    void insertVertex(double * inputKey,int length);
    void insertVertex(trieNode **otherRootNode,double * inputKey,int length);
    bool insertVertex(trieNode **prevNode,trieNode **currentNode,double * inputKey,int length,int prevDim,int currentDim,int matchCount);
    
    void printTrie();
    void printTrie(trieNode *currentNode,double **key, int dim);
    void deleteTrie();
    void deleteTrie(trieNode *currentNode,int dim);
    
    double EVMSize();
    void EVMSize(trieNode *currentNode,double *size);

    bool compareEVM(nDEVM *otherEVM);
    bool compareByCouplets(nDEVM *otherEVM);
    void compareTrie(trieNode **currentNode,trieNode **otherCurrentNode,bool *compare);

    nDEVM *cloneEVM();
    trieNode *cloneTrie();
    void cloneTrie(trieNode **prevNode,trieNode **currentNode,trieNode **copyPrevNode,trieNode **copyCurrentNode);

    void removeVertex(double *key);
    bool removeVertex(trieNode **prevNode,trieNode **currentNode,double *key,int currentDim);

    bool existsVertex(double * inputKey,int length);
    bool existsVertex(trieNode **currentNode,double * inputKey,int length,int currentDim);

    trieNode *getSubTrie(double key);
    trieNode *getSubTrie(trieNode **currentNode,double key);
    nDEVM* couplet(double key);
    trieNode *getSubTrie2(double key);
    trieNode *getSubTrie2(trieNode **currentNode,double key);

    nDEVM* mergeXOR(nDEVM *secondTrie);
    trieNode *XORTrie(trieNode **secondTrie,int dim);
    void XORTrie(trieNode **resultTrie,trieNode **currentNode,double **key,int dim);

    void rawFileToEVM(string fileName,int x1,int x2,int x3);

    void populate3DVoxel(double **inputKey);
    void populate2DVoxel(double **inputKey);
    void populateVoxel(double **inputKey,int dim,int currentDim,int offset);
//    void populateVoxel2(double **inputKey,int dim,int currentDim);

    void EVMFile(int index);
    void EVMFile(ofstream *EVMFile,trieNode *currentNode,double **key, int dim);
    
    string vectorToString(double **vector,int size);
    string vectorToString2(double **vector,int size);
    
    void putCouplet(nDEVM * couplet);
    void putCouplet(trieNode** prevNode,trieNode **currentNode,trieNode *coupletRoot);
    void putSection(nDEVM * section);
    
    void setCoord(double coord);
    double getCoord();
    
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
    
    int getDimDepth();
    int getDimDepth(trieNode* currentNode,int dim);

//-- Operaciones regularizadas
    nDEVM* booleanOperation(nDEVM* evm2,string op, int n);
    nDEVM* booleanOperation(nDEVM *evm1, nDEVM* evm2, string op, int n);
    void nextObject(nDEVM *p, nDEVM *q,double *coord,bool *fromP, bool *fromQ);
    // Operacion basica entre dos segmentos 1D.
//    void generalUnionOperation(trieNode* section1, trieNode* section2,nDEVM **result);
    void condInsertVertex(double * inputKey,int length);
    nDEVM* unionOperation(nDEVM* section1, nDEVM* section2);
    void unionOperation(trieNode* segment1, trieNode* segment2,nDEVM **result);
    bool putCoupletByOp(string op,int argPosition);
    
    //Pruebas de operaciones Booleanas
    void load3DRawFile(string fileName,int voxelSize);
    void load2DRawFile(string fileName,int voxelSize);
    void loadnDRawFile(string fileName,int voxelSize,int dim);
    void voxelizeRawFile(double **voxelInput,ifstream *inputFile,int voxelSize,int dim, int currentDim);
};

#endif	/* TRIETREE_H */

