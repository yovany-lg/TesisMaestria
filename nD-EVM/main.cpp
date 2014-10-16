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
bool test3D(int size, int testIndex);

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

    srand( time( NULL ) );
    
    for(int i = 0; i < 2500; i++){
        if(!test3D(5,i))
            break;
    }
    
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

bool test2D(int size, int testIndex){
    nDEVM *hvEVM1,*hvEVM2,*hvResult,*evmResult;
    unsigned char value1, value2,result;    // Las hypervoxelizaciones tienen dimensiones menores a 8 bits...
    bool hyperBox1,hyperBox2,compare;
    double *voxelInput = new double [3];
    
    hvEVM1 = new nDEVM();
    hvEVM2 = new nDEVM();
    hvResult = new nDEVM();
    ofstream hv1File("3DTest/hv1File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hv2File("3DTest/hv2File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hvResultFile("3DTest/hvUnionResult"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
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

    for(int x3 = 0; x3 < size; x3++){   // Dimension 3
        voxelInput[2] = x3;
        
        for(int x2 = 0; x2 < size; x2++){
            voxelInput[1] = x2;
            
            for(int x1 = 0; x1 < size; x1++){
                value1 = rand() % 2;
                value2 = rand() % 2;
                //cout<< "sizeof value1: "<<sizeof value1<<endl;
                hv1File.write((char *) & value1, sizeof value1);
                hv2File.write((char *) & value2, sizeof value2);
                
                // Conversion al EVM
                if(value1 == 1){
                    voxelInput[0] = x1;
                    hvEVM1->populateVoxel(&voxelInput,3,0,0);
                }

                if(value2 == 1){
                    voxelInput[0] = x1;
                    hvEVM2->populateVoxel(&voxelInput,3,0,0);
                }

                // Si la hyperBox esta llena o vacia
                hyperBox1 = value1 == 1;
                hyperBox2 = value2 == 1;

                // Operacion de union, OR
                if(hyperBox1 or hyperBox2){
                    //hvResultFile[x3][x2][x1] = 1;
                    result = 1;
                    hvResultFile.write((char *) & result, sizeof result);
                    // Conversion al EVM
                    voxelInput[0] = x1;
                    hvResult->populateVoxel(&voxelInput,3,0,0);
                }else{
//                    hvResultFile[x3][x2][x1] = 0;
                    result = 0;
                    hvResultFile.write((char *) & result, sizeof result);
                }
            }
        }
    }
    
    hv1File.close();
    hv2File.close();
    hvResultFile.close();

    evmResult = hvEVM1->booleanOperation(hvEVM2,"union",3);
    
//    evmResult->EVMFile(3003);
//    hvEVM1->EVMFile(3001);
//    hvEVM2->EVMFile(3002);
//    hvResult->EVMFile(3000);
    
    compare = evmResult->compareEVM(hvResult);
    if(compare){
        cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs: True"<<endl;
        return true;
    }else{
        cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs: False"<<endl;
        return false;
    }
    
//    nDEVM *loadedFile = new nDEVM();
//    loadedFile->load3DRawFile("3DTest/hvUnionResult.raw",size);
//    cout<<"Comparación del EVM formado con el archivo binario resultante y el EVM resultante:  "<<loadedFile->compareEVM(hvResult);
}


bool test3D(int size, int testIndex){
    nDEVM *hvEVM1,*hvEVM2,*hvResult,*evmResult;
    unsigned char value1, value2,result;    // Las hypervoxelizaciones tienen dimensiones menores a 8 bits...
    bool hyperBox1,hyperBox2,compare;
    double *voxelInput = new double [3];
    
    hvEVM1 = new nDEVM();
    hvEVM2 = new nDEVM();
    hvResult = new nDEVM();
    ofstream hv1File("3DTest/hv1File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hv2File("3DTest/hv2File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hvResultFile("3DTest/hvUnionResult"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
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

    srand( time( NULL ) );
    for(int x3 = 0; x3 < size; x3++){   // Dimension 3
        voxelInput[2] = x3;
        
        for(int x2 = 0; x2 < size; x2++){
            voxelInput[1] = x2;
            
            for(int x1 = 0; x1 < size; x1++){
                value1 = rand() % 2;
                value2 = rand() % 2;
                //cout<< "sizeof value1: "<<sizeof value1<<endl;
                hv1File.write((char *) & value1, sizeof value1);
                hv2File.write((char *) & value2, sizeof value2);
                
                // Conversion al EVM
                if(value1 == 1){
                    voxelInput[0] = x1;
                    hvEVM1->populateVoxel(&voxelInput,3,0,0);
                }

                if(value2 == 1){
                    voxelInput[0] = x1;
                    hvEVM2->populateVoxel(&voxelInput,3,0,0);
                }

                // Si la hyperBox esta llena o vacia
                hyperBox1 = value1 == 1;
                hyperBox2 = value2 == 1;

                // Operacion de union, OR
                if(hyperBox1 or hyperBox2){
                    //hvResultFile[x3][x2][x1] = 1;
                    result = 1;
                    hvResultFile.write((char *) & result, sizeof result);
                    // Conversion al EVM
                    voxelInput[0] = x1;
                    hvResult->populateVoxel(&voxelInput,3,0,0);
                }else{
//                    hvResultFile[x3][x2][x1] = 0;
                    result = 0;
                    hvResultFile.write((char *) & result, sizeof result);
                }
            }
        }
    }
    
    hv1File.close();
    hv2File.close();
    hvResultFile.close();

    evmResult = hvEVM1->booleanOperation(hvEVM2,"union",3);
    
//    evmResult->EVMFile(3003);
//    hvEVM1->EVMFile(3001);
//    hvEVM2->EVMFile(3002);
//    hvResult->EVMFile(3000);
    
    compare = evmResult->compareEVM(hvResult);
    if(compare){
        cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs: True"<<endl;
        return true;
    }else{
        cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs: False"<<endl;
        return false;
    }
    
//    nDEVM *loadedFile = new nDEVM();
//    loadedFile->load3DRawFile("3DTest/hvUnionResult.raw",size);
//    cout<<"Comparación del EVM formado con el archivo binario resultante y el EVM resultante:  "<<loadedFile->compareEVM(hvResult);
}

string vectorToString(int **vector,int size){
    string output="";
    for(int i =0;i<size;i++)
        output+=to_string((*vector)[i])+"  ";
    return output;
}
