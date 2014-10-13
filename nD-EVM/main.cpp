/* 
 * File:   main.cpp
 * Author: yova
 *
 * Created on 12 de agosto de 2014, 04:23 PM
 */
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "nDEVM.h"

void test2D(int size);
string vectorToString(int **vector,int size);
void hvUnion2D(int **hv1,int **hv2, int **result,int size,ofstream *resultFile);
nDEVM *hvEVM(int **hv, int size, int dim);
void test3D(int size);

template <typename T> std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

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
    
//    nDEVM * evm1 = new nDEVM();
//    double inputKey [] = {0};
//    evm1->insertVertex(inputKey,1);
//    double inputKey2 [] = {2};
//    evm1->insertVertex(inputKey2,1);
//    
//    double inputKey3 [] = {3};
//    evm1->insertVertex(inputKey3,1);
//    double inputKey4 [] = {5};
//    evm1->insertVertex(inputKey4,1);
//    
//    double inputKey5 [] = {7};
//    evm1->insertVertex(inputKey5,1);
//    double inputKey6 [] = {9};
//    evm1->insertVertex(inputKey6,1);    
//    
//    nDEVM * evm2 = new nDEVM();
//    inputKey [0] = 6;
//    evm2->insertVertex(inputKey,1);
//    inputKey2 [0] = 8;
//    evm2->insertVertex(inputKey2,1);
//    
//    inputKey3 [0] = 10;
//    evm2->insertVertex(inputKey3,1);
//    inputKey4 [0] = 12;
//    evm2->insertVertex(inputKey4,1);
//    
//    evm1->booleanOperation(evm2,"union",1);
    
    
    test3D(4);
    
    return 0;
}

void test2D(int size){
    nDEVM *hvEVM1,*hvEVM2,*hvEVMRes,*evmResult;
    int **hv1,**hv2,**result;
    bool hyperBox1,hyperBox2;
    double *inputKey = new double [2];
    
    hvEVM1 = new nDEVM();
    hvEVM2 = new nDEVM();
    hvEVMRes = new nDEVM();
    ofstream hvFile1("2DTest/hvFile1.txt");
    ofstream hvFile2("2DTest/hvFile2.txt");
    ofstream hvResult("2DTest/hvUnionResult.txt");
    
    if(!hvFile1.is_open()){
        cout << "Could not open file!" << '\n';    
        return;
    }
    if(!hvFile2.is_open()){
        cout << "Could not open file!" << '\n';    
        return;
    }
    if(!hvResult.is_open()){
        cout << "Could not open file!" << '\n';    
        return;
    }

    // Inicializacion aleatoria y almacenamiento
    hv1 = new int *[size];
    hv2 = new int *[size];
    result = new int *[size];
    srand( time( NULL ) );
//    cout<< "HV1: \n";
    for(int i = 0; i < size; i++){
        hv1[i] = new int [size];
        hv2[i] = new int [size];
        result[i] = new int [size];
        inputKey[0] = i;
        for(int j = 0; j < size; j++){
            hv1[i][j] = rand() % 2;
            hv2[i][j] = rand() % 2;
            // Si la hyperBox esta llena o vacia
            hyperBox1 = hv1[i][j] == 1;
            hyperBox2 = hv2[i][j] == 1;
            // Operacion de union, OR
            if(hyperBox1 or hyperBox2){
                result[i][j] = 1;
                // Conversion al EVM
                inputKey[1] = j;
                hvEVMRes->populate2DVoxel(&inputKey);
            }else{
                result[i][j] = 0;
            }
            // Conversion al EVM
            if(hv1[i][j] == 1){
                inputKey[1] = j;
                hvEVM1->populate2DVoxel(&inputKey);
            }
            
            if(hv2[i][j] == 1){
                inputKey[1] = j;
                hvEVM2->populate2DVoxel(&inputKey);
            }
        }
        hvFile1<<vectorToString(&(hv1[i]), size)<<endl;
        hvFile2<<vectorToString(&(hv2[i]), size)<<endl;

//        cout<<vectorToString(&(hv1[i]), size)<<endl;
        hvResult<<vectorToString(&(result[i]),size)<<endl;
    }
    hvFile1.close();
    hvFile2.close();
    //hvUnion2D(hv1,hv2,result,size,&hvResult);
    hvResult.close();
    
    evmResult = hvEVM1->booleanOperation(hvEVM2,"union",2);
    
    evmResult->setCoord(0);
    evmResult->EVMFile(3003);

    hvEVM1->setCoord(0);
    hvEVM1->EVMFile(3001);

    hvEVM2->setCoord(0);
    hvEVM2->EVMFile(3002);

    hvEVMRes->setCoord(0);
    hvEVMRes->EVMFile(3000);
    
    cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs:  "<<evmResult->compareEVM(hvEVMRes);
}

void test3D(int size){
    nDEVM *hvEVM1,*hvEVM2,*hvEVMRes,*evmResult;
    int ***hv1,***hv2,***hvResult;
    bool hyperBox1,hyperBox2;
    double *inputKey = new double [3];
    
    hvEVM1 = new nDEVM();
    hvEVM2 = new nDEVM();
    hvEVMRes = new nDEVM();

    // Inicializacion aleatoria y almacenamiento
    hv1 = new int **[size]; // Arreglo de dobles apuntadores [**x3] -> [[*x2] -> [x1]] => apuntan a mallas 2D
    hv2 = new int **[size];
    hvResult = new int **[size];
    srand( time( NULL ) );
    for(int x3 = 0; x3 < size; x3++){   // Dimension 3
        hv1[x3] = new int *[size];   // Arreglo de apuntadores [*x2] -> [x1] => apuntan a vectores
        hv2[x3] = new int *[size];
        hvResult[x3] = new int *[size];
        inputKey[2] = x3;
        
        for(int x2 = 0; x2 < size; x2++){
            hv1[x3][x2] = new int [size]; // Arreglo [x1] => vectores
            hv2[x3][x2] = new int [size]; 
            hvResult[x3][x2] = new int [size]; 
            inputKey[1] = x2;
            
            for(int x1 = 0; x1 < size; x1++){
                hv1[x3][x2][x1] = rand() % 2;
                hv2[x3][x2][x1] = rand() % 2;
                // Si la hyperBox esta llena o vacia
                hyperBox1 = hv1[x3][x2][x1] == 1;
                hyperBox2 = hv2[x3][x2][x1] == 1;
                
                // Operacion de union, OR
                if(hyperBox1 or hyperBox2){
                    hvResult[x3][x2][x1] = 1;
                    // Conversion al EVM
                    inputKey[0] = x1;
                    hvEVMRes->populateVoxel(&inputKey,3,0,0);
                }else{
                    hvResult[x3][x2][x1] = 0;
                }
                // Conversion al EVM
                if(hv1[x3][x2][x1] == 1){
                    inputKey[0] = x1;
                    hvEVM1->populateVoxel(&inputKey,3,0,0);
                }

                if(hv2[x3][x2][x1] == 1){
                    inputKey[0] = x1;
                    hvEVM2->populateVoxel(&inputKey,3,0,0);
                }
            }
        }
    }
    
    evmResult = hvEVM1->booleanOperation(hvEVM2,"union",3);
    
    evmResult->EVMFile(3003);

    hvEVM1->EVMFile(3001);

    hvEVM2->EVMFile(3002);

    hvEVMRes->EVMFile(3000);
    
    cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs:  "<<evmResult->compareEVM(hvEVMRes);
}

// Se ahorra esta funcion...
void hvUnion2D(int **hv1,int **hv2, int **result,int size,ofstream *resultFile){
    bool hb1,hb2;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            hb1 = hv1[i][j] == 1;
            hb2 = hv2[i][j] == 1;
            if(hb1 or hb2)
                result[i][j] = 1;
            else
                result[i][j] = 0;                
        }
        *resultFile<<vectorToString(&(result[i]),size)<<endl;
    }
}

nDEVM *hvEVM(int **hv, int size, int dim){
    nDEVM *evm = new nDEVM();
    double *inputKey = new double [dim];
    for(int i = 0; i < size; i++){
        inputKey[0] = i;
        for(int j = 0; j < size; j++){
            if(hv[i][j] == 1){
                inputKey[1] = j;
                evm->populate2DVoxel(&inputKey);
            }
        }
    }
}

string vectorToString(int **vector,int size){
    string output="";
    for(int i =0;i<size;i++)
        output+=to_string((*vector)[i])+"  ";
    return output;
}
