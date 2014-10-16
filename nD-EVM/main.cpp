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
bool test2D(int size, int testIndex);

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
        if(!test2D(5,i))
            break;
    }
    //test2D(5,0);
    return 0;
}

bool test2D(int size, int testIndex){
    nDEVM *hvEVM1,*hvEVM2,*hvResult,*evmResult;
    unsigned char value1, value2,result;    // Las hypervoxelizaciones tienen dimensiones menores a 8 bits...
    bool hyperBox1,hyperBox2,compare;
    double *voxelInput = new double [2];
    
    hvEVM1 = new nDEVM();
    hvEVM2 = new nDEVM();
    hvResult = new nDEVM();
    ofstream hv1File("2DTest/hv1File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hv2File("2DTest/hv2File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hvResultFile("2DTest/hvUnionResult"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
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

    for(int x2 = 0; x2 < size; x2++){ // Dimension 2
        voxelInput[1] = x2;

        for(int x1 = 0; x1 < size; x1++){
            value1 = rand() % 2;
            value2 = rand() % 2;
            hv1File.write((char *) & value1, sizeof value1);
            hv2File.write((char *) & value2, sizeof value2);

            // Conversion al EVM
            if(value1 == 1){
                voxelInput[0] = x1;
                hvEVM1->populateVoxel(&voxelInput,2,0,0);
            }

            if(value2 == 1){
                voxelInput[0] = x1;
                hvEVM2->populateVoxel(&voxelInput,2,0,0);
            }

            // Si la hyperBox esta llena o vacia
            hyperBox1 = value1 == 1;
            hyperBox2 = value2 == 1;

            // Operacion de union, OR
            if(hyperBox1 or hyperBox2){
                result = 1;
                hvResultFile.write((char *) & result, sizeof result);
                // Conversion al EVM
                voxelInput[0] = x1;
                hvResult->populateVoxel(&voxelInput,2,0,0);
            }else{
                result = 0;
                hvResultFile.write((char *) & result, sizeof result);
            }
        }
    }
    
    hv1File.close();
    hv2File.close();
    hvResultFile.close();

    evmResult = hvEVM1->booleanOperation(hvEVM2,"union",2);
    
    compare = evmResult->compareEVM(hvResult);
    if(compare){
        cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs: True"<<endl;
//        nDEVM *loadedFile = new nDEVM();
//        loadedFile->load2DRawFile("2DTest/hvUnionResult"+to_string(testIndex)+".raw",size);
//        cout<<"Comparación del EVM formado con el archivo binario resultante y el EVM resultante:  "<<loadedFile->compareEVM(hvResult);
        return true;
    }else{
        cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs: False"<<endl;
        return false;
    }
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
                    result = 1;
                    hvResultFile.write((char *) & result, sizeof result);
                    // Conversion al EVM
                    voxelInput[0] = x1;
                    hvResult->populateVoxel(&voxelInput,3,0,0);
                }else{
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
