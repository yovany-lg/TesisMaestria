/* 
 * File:   trieTree.h
 * Author: yova
 *
 * Created on 24 de octubre de 2014, 12:57 PM
 */

#ifndef TRIETREE_H
#define	TRIETREE_H

struct trieNode{
    double value;  //Valor de la dimension actual
    trieNode *nextDim;
    trieNode *nextTrieNode;
};

using namespace std;

class TrieTree {
public:
    trieNode *rootNode;
    trieNode **coupletIndex;

    TrieTree();
    TrieTree(trieNode *node);
    TrieTree(const TrieTree& orig);
    virtual ~TrieTree();
    
    void deleteTrie();
    void deleteTrie(trieNode *currentNode);
    void resetCoupletIndex();
    bool isEmpty();
    bool endTrie();
    
    void insertVertex(double * inputKey,int length);
    bool insertVertex(trieNode **prevNode,trieNode **currentNode,double * inputKey,
        int length,int prevDim,int currentDim,int matchCount);
    bool existsVertex(double * inputKey,int length);
    bool existsVertex(trieNode **currentNode,double * inputKey,int length,int currentDim);
    void clone(trieNode **prevNode,trieNode **currentNode,trieNode **copyPrevNode,trieNode **copyCurrentNode);
    void removeVertex(double *key);
    bool removeVertex(trieNode **prevNode,trieNode **currentNode,double *key,int currentDim);
    
    bool compare(TrieTree *otherTrie);
    void compare(trieNode **currentNode,trieNode **otherCurrentNode,bool *compare);
    TrieTree *clone();
    TrieTree *mergeXOR(TrieTree* otherTrie);
    TrieTree *XOR(TrieTree *otherTrie);
    void XOR(TrieTree **resultTrie,trieNode **currentNode,double **key,int dim);
    
    void putCouplet(TrieTree * couplet);
    void putCouplet2(TrieTree* couplet);
    void putCouplet(trieNode** prevNode,trieNode **currentNode,trieNode *coupletRoot);
    TrieTree *readCouplet();
    void setCoord(double coord);
    double getCoord();
    double coupletCoord();
    
    TrieTree* unionOperation(TrieTree* section1, TrieTree* section2);
    void unionOperation(trieNode* section1, trieNode* section2,TrieTree **result);
    TrieTree *intersectionOperation(TrieTree* section1, TrieTree* section2);
    void intersectionOperation(trieNode* section1, trieNode* section2,trieNode **result);
    TrieTree *differenceOperation(TrieTree* section1, TrieTree* section2);
    void differenceOperation(trieNode** section1, trieNode** section2,trieNode **result);
    void mergeSegments(trieNode ***currentSegment, trieNode *otherSegment);
    TrieTree* xorOperation(TrieTree* section1, TrieTree* section2);
    void xorOperation(trieNode* section1, trieNode* section2,trieNode **result);
    
    trieNode *cloneSegment(trieNode *segment);
    bool putCoupletByOp(string op,int argPosition);
    void condInsertVertex(double * inputKey,int length);
    
    void printTrie();
    void printTrie(trieNode *currentNode,double **key, int dim);
    double size();
    void size(trieNode *currentNode,double *size);
    int dimDepth();
    int dimDepth(trieNode* currentNode,int dim);
    string vectorToString(double **vector,int size);
    string vectorToString2(double **vector,int size);
    
    // - Cargar Videos e Imagenes
    void loadImage(string fileName);
private:
};

#endif	/* TRIETREE_H */

