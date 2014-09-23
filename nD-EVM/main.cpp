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
//    nDEVM * evm1 = new nDEVM();
//    double inputKey [] = {1,1,1,2};
//    evm1->insertVertex(inputKey,4);
//    double inputKey2 [] = {1,1,1,3};
//    evm1->insertVertex(inputKey2,4);
//    double inputKey3 [] = {1,3,2,0};
//    evm1->insertVertex(inputKey3,4);
//    double inputKey4 [] = {3,0,0,3};
//    evm1->insertVertex(inputKey4,4);
//    double inputKey5 [] = {2,2,1,0};
//    evm1->insertVertex(inputKey5,4);
//    double inputKey6 [] = {3,1,0,2};
//    evm1->insertVertex(inputKey6,4);
//
//    cout<<"EVM original"<<endl;
//    evm1->printTrie();
//    cout<<endl;
//    
//    nDEVM * evm2 = NULL;
//    evm2 = evm1->cloneEVM();
//    cout<<"Copia"<<endl;
//    evm2->printTrie();
//    cout<<endl;
//
//    cout<<"Comparando los EVMs: "<<evm1->compareEVM(evm2)<<endl;
//    
//    cout<<"Insertar el primer vector nuevamente en el original"<<endl;
//    evm1->insertVertex(inputKey,4);
//    evm1->printTrie();
//    cout<<endl;
//    
//    cout<<"Comparando los EVMs: "<<evm1->compareEVM(evm2)<<endl;
//    
//    cout<<"Obteniendo un Couplet: "<<endl;
//    nDEVM *evmCouplet = evm2->couplet(0);
//    evmCouplet->printTrie();
//    cout<<endl;
//    
//    nDEVM * xorEVM = evm1->mergeXOR(evm2);
//    
//    cout<<"Operador XOR: "<<endl;
//    xorEVM->printTrie();
//    cout<<endl;

//    nDEVM *evm = new nDEVM();
//    string fileName = "VL-baby-(256x256x98)-(1,1,2).raw";
//    evm->rawFileToEVM(fileName,256,256,98);

//    nDEVM *evm2 = new nDEVM();
//    string fileName2 = "VL-vismale-(128x256x256)-(1.5,1,1).raw";
//    evm2->rawFileToEVM(fileName2,128,256,256);
//    //cout<<"Tamaño del EVM: "<<evm2->EVMSize()<<endl;
//
////    nDEVM * xorEVM = evm->XOR(evm2,4);
////    cout<<"Operador XOR: "<<endl;
//
////    nDEVM * newEVM = new nDEVM();
////    for(int i = 0; i < 10; i++){
////        nDEVM * tempEVM = evm2->couplet((double)i);
////        newEVM->putCouplet(tempEVM,(double)i);
//////        newEVM->printTrie();
//////        cout<<endl;
////    }
//
//    double couplet;
//    cout<<"Elige un Couplet:"<<endl;
//    cin >> couplet;
//    while(couplet != -1){
//        nDEVM *EVMCouplet = evm2->couplet(couplet);
//        nDEVM* EVMnewCoord = EVMCouplet->setCoord(0);
//        EVMnewCoord->EVMFile();
//        cout<<"Elige un Couplet:"<<endl;    
//        cin >> couplet;
//    }

//    nDEVM * evm1 = new nDEVM();
//    double inputKey [] = {0,0,0};
//    evm1->insertVertex(inputKey,3);
//    double inputKey2 [] = {1,0,0};
//    evm1->insertVertex(inputKey2,3);
//    double inputKey3 [] = {2,0,0};
//    evm1->insertVertex(inputKey3,3);
//    double inputKey4 [] = {4,0,0};
//    evm1->insertVertex(inputKey4,3);
//    double inputKey5 [] = {3,0,-1};
//    evm1->insertVertex(inputKey5,3);
//    double inputKey6 [] = {4,0,-1};
//    evm1->insertVertex(inputKey6,3);
//    double inputKey7 [] = {0,0,1};
//    evm1->insertVertex(inputKey7,3);
//    double inputKey8 [] = {1,0,1};
//    evm1->insertVertex(inputKey8,3);
//    double inputKey9 [] = {2,0,1};
//    evm1->insertVertex(inputKey9,3);
//    double inputKey10 [] = {3,0,1};
//    evm1->insertVertex(inputKey10,3);
//    
//    double inputKey11 [] = {0,1,0};
//    evm1->insertVertex(inputKey11,3);
//    double inputKey12 [] = {2,1,0};
//    evm1->insertVertex(inputKey12,3);
//    double inputKey13 [] = {1,1,-1};
//    evm1->insertVertex(inputKey13,3);
//    double inputKey14 [] = {3,1,-1};
//    evm1->insertVertex(inputKey14,3);
//    double inputKey15 [] = {0,1,1};
//    evm1->insertVertex(inputKey15,3);
//    double inputKey16 [] = {1,1,1};
//    evm1->insertVertex(inputKey16,3);
//    double inputKey17 [] = {2,1,1};
//    evm1->insertVertex(inputKey17,3);
//    double inputKey18 [] = {3,1,1};
//    evm1->insertVertex(inputKey18,3);
//    
//    double inputKey19 [] = {1,2,0};
//    evm1->insertVertex(inputKey19,3);
//    double inputKey20 [] = {4,2,0};
//    evm1->insertVertex(inputKey20,3);
//    double inputKey21 [] = {1,2,-1};
//    evm1->insertVertex(inputKey21,3);
//    double inputKey22 [] = {4,2,-1};
//    evm1->insertVertex(inputKey22,3);
//    
//    evm1->EVMFile(0);
//    cout<<"Trie Original: "<<endl;
//    evm1->printTrie();
    //cout<<evm1->getDimDepth()<<endl;
    
    nDEVM *evm1 = new nDEVM();
    string fileName2 = "VL-vismale-(128x256x256)-(1.5,1,1).raw";
    evm1->rawFileToEVM(fileName2,128,256,256);

    nDEVM* evm2 = evm1->EVMSectionSequence();

    cout<<endl<<"Section Sequence Obtained..."<<endl;
    //evm2->printTrie();
    
    nDEVM* evm3 = evm2->EVMCoupletSequence();
    cout<<endl<<"Couplet Sequence obtained..."<<endl;
    //evm3->printTrie();
    
    cout<<"Comparacion del EVM de la secuencia de couplets y el EVM original: "
            <<evm1->compareByCouplets(evm3)<<endl;
    

//    nDEVM *evm2 = new nDEVM();
//    string fileName2 = "VL-vismale-(128x256x256)-(1.5,1,1).raw";
//    evm2->rawFileToEVM(fileName2,128,256,256);
//    
//    evm2->EVMSectionSequence();
    
    return 0;
}

