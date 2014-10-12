/* 
 * File:   main.cpp
 * Author: yova
 *
 * Created on 12 de agosto de 2014, 04:23 PM
 */
#include <cstdlib>
#include <iostream>

#include "nDEVM.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {   
    // Pruebas para verificar la secuencia de secciones y de couplets
    
//    nDEVM *evm1 = new nDEVM();
//    string fileName2 = "VL-vismale-(128x256x256)-(1.5,1,1).raw";
//    evm1->rawFileToEVM(fileName2,128,256,256);
//
//    nDEVM* evm2 = evm1->EVMSectionSequence();
//
//    cout<<endl<<"Section Sequence Obtained..."<<endl;
//    //evm2->printTrie();
//    
//    nDEVM* evm3 = evm2->EVMCoupletSequence();
//    cout<<endl<<"Couplet Sequence obtained..."<<endl;
//    //evm3->printTrie();
//    
//    cout<<"Comparacion del EVM de la secuencia de couplets y el EVM original: "
//            <<evm1->compareByCouplets(evm3)<<endl;
    
//    nDEVM *evm2 = new nDEVM();
//    string fileName2 = "VL-vismale-(128x256x256)-(1.5,1,1).raw";
//    evm2->rawFileToEVM(fileName2,128,256,256);
//    
//    evm2->EVMSectionSequence();
    
    // Pruebas de operaciones booleanas
    
    nDEVM * evm1 = new nDEVM();
    double inputKey [] = {0};
    evm1->insertVertex(inputKey,1);
    double inputKey2 [] = {2};
    evm1->insertVertex(inputKey2,1);
    
    double inputKey3 [] = {3};
    evm1->insertVertex(inputKey3,1);
    double inputKey4 [] = {5};
    evm1->insertVertex(inputKey4,1);
    
    double inputKey5 [] = {7};
    evm1->insertVertex(inputKey5,1);
    double inputKey6 [] = {9};
    evm1->insertVertex(inputKey6,1);    
    
    nDEVM * evm2 = new nDEVM();
    inputKey [0] = 6;
    evm2->insertVertex(inputKey,1);
    inputKey2 [0] = 8;
    evm2->insertVertex(inputKey2,1);
    
    inputKey3 [0] = 10;
    evm2->insertVertex(inputKey3,1);
    inputKey4 [0] = 12;
    evm2->insertVertex(inputKey4,1);
    
    evm1->booleanOperation(evm2,"union",1);

    return 0;
}

