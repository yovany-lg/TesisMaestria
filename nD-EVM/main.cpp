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
#include <sys/time.h>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

#include "nDEVM.h"

void test2D(int size);
string vectorToString(int **vector,int size);
void hvUnion2D(int **hv1,int **hv2, int **result,int size,ofstream *resultFile);
nDEVM *hvEVM(int **hv, int size, int dim);
bool test3D(int size, int testIndex);
bool test2D(int size, int testIndex);
bool test(int size,int dim, int testIndex,ofstream *executionTime);
void hvGeneration(double **voxelInput,int size,nDEVM **hvEVM1,nDEVM **hvEVM2, nDEVM **hvResult,
        ofstream *hv1File,ofstream *hv2File,ofstream *hvResultFile, int dim,int currentDim);
string vectorToStringD(double **vector,int size);


typedef unsigned long long timestamp_t;
static timestamp_t get_timestamp(){
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

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

    
    // Pruebas Basicas de operaciones booleanas
    
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

//    nDEVM * evm1 = new nDEVM();
//    double inputKey [] = {1,1,1,2};
//    evm1->insertVertex(inputKey,4);
//    double inputKey2 [] = {1,1,1,3};
//    evm1->insertVertex(inputKey2,4);    
//    double inputKey3 [] = {1,3,2,0};
//    evm1->insertVertex(inputKey3,4);
//    double inputKey4 [] = {2,2,1,0};
//    evm1->insertVertex(inputKey4,4);
//    double inputKey5 [] = {3,0,0,3};
//    evm1->insertVertex(inputKey5,4);
//    double inputKey6 [] = {3,1,0,2};
//    evm1->insertVertex(inputKey6,4);
//    
//    evm1->printTrie();
//    evm1->deleteEVM();

    // Test de Operaciones Booleanas
    srand( time( NULL ) );
    int dim = 7;
    // Archivo donde se guarda el tiempo de ejecución...
    ofstream executionTime(to_string(dim)+"DTest/executionTime.txt",ios_base::out | ios_base::app);
    if (!executionTime.is_open()){
        cerr << "Can't open "+to_string(dim)+"DTest/executionTime.txt file for output.\n";
        return false;
    }
    
    for(int i = 0; i < 2500; i++){
        if(!test(5,dim,i,&executionTime)){
            cout<<"ERROR!!!"<<endl;
            break;
        }
    }
    executionTime.close();
    return 0;
}

/**
 * Pruebas de operaciones booleanas para n dimensiones...
 * @param size: El tamaño de las hyper-voxelizaciones.
 * @param dim: La dimensión del espacio.
 * @param testIndex: Índice con el que serán guardados los archivos binarios.
 * @return 
 */
bool test(int size,int dim, int testIndex,ofstream *executionTime){
    nDEVM *hvEVM1,*hvEVM2,*hvResult,*evmResult;
    bool compare;
    double *voxelInput = new double [dim];
    
    hvEVM1 = new nDEVM();
    hvEVM2 = new nDEVM();
    hvResult = new nDEVM();
    ofstream hv1File(to_string(dim)+"DTest/hv1File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hv2File(to_string(dim)+"DTest/hv2File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hvResultFile(to_string(dim)+"DTest/hvUnionResult"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    if(!hv1File.is_open()){
        cout << "Could not open file!" << '\n';    
        return false;
    }
    if(!hv2File.is_open()){
        cout << "Could not open file!" << '\n';    
        return false;
    }
    if(!hvResultFile.is_open()){
        cout << "Could not open file!" << '\n';    
        return false;
    }
    
    hvGeneration(&voxelInput,size,&hvEVM1,&hvEVM2,&hvResult,&hv1File,&hv2File,&hvResultFile,dim,dim);

    hv1File.close();
    hv2File.close();
    hvResultFile.close();

    timestamp_t t0 = get_timestamp();
//    clock_t clockTicks;
//    clockTicks = clock();
    evmResult = hvEVM1->booleanOperation(hvEVM2,"union",dim);

//    clockTicks = clock() - clockTicks;
//    float totalTime = ((float)clockTicks)/CLOCKS_PER_SEC;
    timestamp_t t1 = get_timestamp();
    double secs = (t1 - t0) / 1000000.0L;
    *executionTime<<hvEVM1->EVMSize()<<"\t"<<hvEVM2->EVMSize()<<"\t"<<secs<<endl;

    compare = evmResult->compareEVM(hvResult);
    delete hvEVM1;
    delete hvEVM2;
    delete evmResult;
    delete hvResult;
    delete voxelInput;
    if(compare){
//        cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs: True"<<endl;

//        nDEVM *loadedFile = new nDEVM();
//        loadedFile->loadnDRawFile(to_string(dim)+"DTest/hvUnionResult"+to_string(testIndex)+".raw",size,dim);
//        cout<<"Comparación del EVM formado con el archivo binario resultante y el EVM resultante:  "<<loadedFile->compareEVM(hvResult);
        return true;
    }else{
//        cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs: False"<<endl;
        return false;
    }
    
}

void hvGeneration(double **voxelInput,int size,nDEVM **hvEVM1,nDEVM **hvEVM2, nDEVM **hvResult,
        ofstream *hv1File,ofstream *hv2File,ofstream *hvResultFile, int dim,int currentDim){
    
    if(currentDim == 0){
        unsigned char value1, value2,result;    // Las hypervoxelizaciones tienen dimensiones menores a 8 bits...
        bool hyperBox1,hyperBox2;
        value1 = rand() % 2;
        value2 = rand() % 2;
        (*hv1File).write((char *) & value1, sizeof value1);
        (*hv2File).write((char *) & value2, sizeof value2);

        // Conversion al EVM
        if(value1 == 1){
            (*hvEVM1)->populateVoxel(voxelInput,dim,0,0);
        }

        if(value2 == 1){
            (*hvEVM2)->populateVoxel(voxelInput,dim,0,0);
        }

        // Si la hyperBox esta llena o vacia
        hyperBox1 = value1 == 1;
        hyperBox2 = value2 == 1;

        // Operacion de union, OR, SEPARAR!!!
        if(hyperBox1 or hyperBox2){
            result = 1;
            (*hvResultFile).write((char *) & result, sizeof result);
            // Conversion al EVM
            (*hvResult)->populateVoxel(voxelInput,dim,0,0);
        }else{
            result = 0;
            (*hvResultFile).write((char *) & result, sizeof result);
        }
        return;
    }
    
    for(int i = 0; i < size; i++){
        (*voxelInput)[currentDim - 1] = i;
        hvGeneration(voxelInput,size,hvEVM1,hvEVM2,hvResult,hv1File,hv2File,hvResultFile,dim,currentDim-1);
    }
}

string vectorToString(int **vector,int size){
    string output="";
    for(int i =0;i<size;i++)
        output+=to_string((*vector)[i])+"  ";
    return output;
}

string vectorToStringD(double **vector,int size){
    string output="";
    for(int i =0;i<size;i++)
        output+=to_string((*vector)[i])+"  ";
    return output;
}
