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
//    nDEVM * xorEVM = evm1->XOR(evm2,4);
//    
//    cout<<"Operador XOR: "<<endl;
//    xorEVM->printTrie();
//    cout<<endl;

//    nDEVM *evm = new nDEVM();
//    string fileName = "VL-baby-(256x256x98)-(1,1,2).raw";
//    evm->rawFileToEVM(fileName,256,256,98);

    nDEVM *evm2 = new nDEVM();
    string fileName2 = "VL-vismale-(128x256x256)-(1.5,1,1).raw";
    evm2->rawFileToEVM(fileName2,128,256,256);
    //cout<<"Tamaño del EVM: "<<evm2->EVMSize()<<endl;

//    nDEVM * xorEVM = evm->XOR(evm2,4);
//    cout<<"Operador XOR: "<<endl;


    double couplet;
    cout<<"Elige un Couplet:"<<endl;
    cin >> couplet;
    while(couplet != -1){
        nDEVM *EVMCouplet = evm2->couplet(couplet);
        EVMCouplet->EVMFile();
        cout<<"Elige un Couplet:"<<endl;    
        cin >> couplet;
    }
    
    return 0;
}

