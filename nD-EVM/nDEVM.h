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
    int dimDepth;   //Profundidad de la dimension 0,...,n-1
    double value;  //Valor de la dimension actual
    trieNode *nextDim;
    trieNode *nextTrieNode;
};

using namespace std;

class nDEVM {
public:
    nDEVM();
    nDEVM(trieNode *node);
    nDEVM(const nDEVM& orig);
    virtual ~nDEVM();
    /*Metodos del Trie*/
    void insertVertex(double * inputKey,int length);
    void insertVertex(trieNode **otherRootNode,double * inputKey,int length);
    bool insertVertex(trieNode **prevNode,trieNode **currentNode,double * inputKey,int length,int iDim,int matchCount);
    
    void printTrie();
    void printTrie(trieNode *currentNode,double **key, int dim);
    
    double EVMSize();
    void EVMSize(trieNode *currentNode,double *size);

    bool compareEVM(nDEVM *otherEVM);
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

    nDEVM* XOR(nDEVM *secondTrie,int dim);
    trieNode *XORTrie(trieNode **secondTrie,int dim);
    void XORTrie(trieNode **resultTrie,trieNode **currentNode,double **key,int dim);

    void rawFileToEVM(string fileName,int x1,int x2,int x3);

    void populate3DVoxel(double **inputKey);
    void populate3DVoxel(double **inputKey,int dim,int currentDim);

    void EVMFile();
    void EVMFile(ofstream *EVMFile,trieNode *currentNode,double **key, int dim);
    
    string vectorToString(double **vector,int size);
    string vectorToString2(double **vector,int size);
private:
    trieNode *rootNode;

};

#endif	/* TRIETREE_H */

