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

class trieTree {
public:
    trieTree();
    trieTree(const trieTree& orig);
    virtual ~trieTree();
private:
    trieNode *rootNode;
    trieNode **coupletIndex;
};

#endif	/* TRIETREE_H */

