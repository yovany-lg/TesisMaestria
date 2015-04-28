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

#include "DataSet.h"
#include "SOM.h"

#include "nDEVM.h"

#include <limits>       // std::numeric_limits

void test2D(int size);
string vectorToString(int **vector,int size);
void hvUnion2D(int **hv1,int **hv2, int **result,int size,ofstream *resultFile);
nDEVM<double> *hvEVM(int **hv, int size, int dim);
bool test3D(int size, int testIndex);
bool test2D(int size, int testIndex);
bool test(int size,int dim,string operation, int testIndex,ofstream *executionTime);
void test(string operation, int dim,int voxelSize,int iterations);
void hvGeneration(string operation,double **voxelInput,int size,nDEVM<double> **hvEVM1,nDEVM<double> **hvEVM2, nDEVM<double> **hvResult,
        ofstream *hv1File,ofstream *hv2File,ofstream *hvResultFile, int dim,int currentDim);
string vectorToStringD(double **vector,int size);
bool booleanOperation(string op,unsigned char value1, unsigned char value2);
void testSequences();
void testOperations();
void testImageLoad();
void testAnimationLoad();
void testUnion();
void testSaving();
void testLoadEVMSeq();
void testImages();
void maskTest();
void maskFrameComparison();
void contentTest();
void shiftTest();
void dcFiles();
void SOMTests();

typedef unsigned long long timestamp_t;

static timestamp_t get_timestamp(){
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

//template <typename T> std::string to_string(T value)
//{
//	std::ostringstream os ;
//	os << value ;
//	return os.str() ;
//}

using namespace std;


/*
 * 
 */
int main(int argc, char** argv) {   
    // Pruebas Basicas de operaciones booleanas

    // Test de Operaciones Booleanas
//    testOperations();
    
    // - Prueba de secuencias de Couplets y Secciones
//    testSequences();
//    testUnion();
    
    // - Test SOM
//    DataSet *dataSet = new DataSet(4,0);
//    string fileName = "DataSets/Iris.csv";
//    dataSet->loadFile(fileName);
//    dataSet->normalize();   // - Agregar guardar archivo normalizado
//    
//    SOM *som = new SOM(dataSet,4,3);
//    som->initialize();
//    som->sampling();
//    som->dataSetClustering();

    // - Test para cargar imagenes
//    testImageLoad();
    
    // - Test para cargar videos
//    testAnimationLoad();
//    testLoadEVMSeq();

    // - Test Savings
//    testSaving();
    
    // - Mask Tests
//    maskTest();
//    maskFrameComparison();
    
    // - Content Tests
//    contentTest();
    
    // - Shift Tests
//    shiftTest();

    // - DC Files tests
//    dcFiles();
    
    // - SOM Tests
//    cout<<"Mod: 0%2000: "<< 0 % 2000<<endl;
    SOMTests();
//  std::cout << std::boolalpha;
//  std::cout << "Minimum value for float: " << std::numeric_limits<float>::min() << '\n';
//  std::cout << "Maximum value for float: " << std::numeric_limits<float>::max() << '\n';
//  std::cout << "float is signed: " << std::numeric_limits<float>::is_signed << '\n';
//  std::cout << "Non-sign bits in float: " << std::numeric_limits<float>::digits << '\n';
//  std::cout << "float has infinity: " << std::numeric_limits<float>::has_infinity << '\n';
    return 0;
}

void SOMTests(){
//    maskTest();
    
    nDEVM<unsigned int> *evmClustering = new nDEVM<unsigned int>();
//    evmClustering->clusterContent(1);
    evmClustering->subAnimClustering(20,3,7);

//    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();
//    mask->maskInit(10,13,5,1,1);
//    
//    evmClustering->clusterContent(1,mask,1590,1595,360,243);

//    SOM *som = new SOM(10);
//    som->loadBinFile("dcFiles/dcFile.dc");
//    som->initialize();
//    som->sampling();
//    som->dataSetClustering();    
    delete evmClustering;
}

void dcFiles(){
//    string fileName = "dcFiles/Part2/dcFile6.dc";
//    ifstream fileInput;
//    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
//    double *dcValue = new double[756];
//    
//    if (! fileInput.is_open()){
//        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
//        return;
//    }
//
//    if(fileInput.read((char *) dcValue, sizeof(double) * 756)){
//        fileName = "dcFiles/Part2/dcFilex.dcx";
//        ofstream outputFile( fileName.c_str(),ios_base::out|ios_base::binary );
//        if ( ! outputFile.is_open() ){    
//            cout << "El archivo no se pudo abrir!" << '\n';    
//            return;
//        } 
//        outputFile.write((char *) dcValue,sizeof(double) * 756);
//        outputFile.close();
////        cout <<"DC: "<<*dcValue <<endl; 
//    }
//
//    
//    fileInput.close();
//    delete dcValue;

    string fileName = "dcFiles/Part2/dcFilex.dcx";
    ifstream fileInput;
    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
    double *dcValue = new double;
    
    if (! fileInput.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    int i = 0;
    while(fileInput.read((char *) dcValue, sizeof(double))){
        cout <<"DC["<<i<<"]: "<<*dcValue <<endl; 
        i++;
    }
    fileInput.close();
    delete dcValue;
}

/**
 * Pruebas del cambio de orden en los ejes...
 */
void shiftTest(){
    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();
    mask->maskInit(100,81,3,1,1);
    cout<<"Original EVM: "<<endl;
    mask->printEVM();

    cout<<"Shifted EVM: "<<endl;    
    nDEVM<unsigned int> *shift = mask->dimLeftShift();
    shift->printEVM();

    cout<<"Shifted EVM: "<<endl;    
    shift = shift->dimLeftShift();
    shift->printEVM();
    
    cout<<"Shifted EVM: "<<endl;    
    shift = shift->dimLeftShift();
    shift->printEVM();
    
    cout<<"Shifted EVM: "<<endl;    
    shift = shift->dimLeftShift();
    shift->printEVM();
}

/**
 * Pruebas del calculo de contenido de un EVM...
 */
void contentTest(){
    nDEVM<unsigned int> *evm2 = new nDEVM<unsigned int>();
    string fileName2 = "DataSets/Sedans/sedan1.raw";
    evm2->rawFileToEVM(fileName2,128,128,128);
    
    nDEVM<unsigned int> *sectionSeq = evm2->EVMSectionSequence();
    
//    evm2->EVMSectionSequence();
//    evm2->readCouplet();
    sectionSeq->readCouplet();
    nDEVM<unsigned int> *temp = sectionSeq->readCouplet();
    
    temp->EVMFile("Sedan",1);
//    temp->discreteCompactness();
    cout<< "Contenido: "<<temp->content()<<", Total Internal Contacts: "<<temp->totalInternalContacts()<<endl; //<<", Área de la frontera: "<<temp->boundaryContent()<<"\n";
    
}

/**
 * Comparacion del resultado de la convolucion con la animacion original
 */
void maskFrameComparison(){
    nDEVM<unsigned int> * frame = new nDEVM<unsigned int>();
    nDEVM<unsigned int> * maskSection = new nDEVM<unsigned int>();
    
    frame->readEVM("frame1591");
    maskSection->readEVM("maskSection1");
    cout<<"Comparacion de Frame y maskSection: "<<frame->compareEVM(maskSection)<<endl;
    
}

/**
 * Inicializacion de la mascara y convolucion con la animacion...
 */
void maskTest(){
    nDEVM<unsigned int> *animMask =  new nDEVM<unsigned int>();
    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();
//    animMask->dcContent(0);
    mask->maskInit(10,13,3,1,1);
    
//    mask->EVMTraslation(2,100);
//    animMask = animMask->maskIntersection(mask,1590,1601);
//    
//    int i = 0;
//    nDEVM<unsigned int> *couplet;
////    nDEVM<unsigned int> *currentSection,*prevSection;
////    currentSection= new nDEVM<unsigned int>();
//    
//    while(!animMask->endEVM()){
//        couplet = animMask->readCouplet();
//        couplet->saveEVM("maskCouplet",i);
////        prevSection = currentSection;
////        currentSection = animMask->getSection(prevSection,couplet);        
////        currentSection->saveEVM("maskSection",i);
////        
////        delete prevSection;
////        delete couplet;
//        i++;
//    }
//    animMask->resetCoupletIndex();
//    
//    for(int i = 0; i <= 5; i++){
//        nDEVM<unsigned int> *frame = new nDEVM<unsigned int>();
//        frame->readEVM("maskCouplet"+to_string(i));
//        frame->EVMFile("maskCouplet",i);
//        delete frame;
//    }
    mask->EVMTraslation(1,2);
    animMask->maskAnimConv(mask,780,880,320,213);
    
}

/**
 * Almacenamiento del EVM en un archivo binario...
 */
void testSaving(){
    nDEVM<double> *evm1 = new nDEVM<double>();
    double inputKey [] = {1,1,1,2};
    evm1->insertVertex(inputKey,4);
    double inputKey2 [] = {1,1,1,3};
    evm1->insertVertex(inputKey2,4);    
    double inputKey3 [] = {1,3,2,0};
    evm1->insertVertex(inputKey3,4);
    double inputKey4 [] = {2,2,1,0};
    evm1->insertVertex(inputKey4,4);
    double inputKey5 [] = {3,0,0,3};
    evm1->insertVertex(inputKey5,4);
    double inputKey6 [] = {3,1,0,2};
    evm1->insertVertex(inputKey6,4);

    evm1->saveEVM("binEVM",-1);
    nDEVM<double> * evm2 = new nDEVM<double>();
    evm2->readEVM("binEVM");
    
    cout<<"EVM Write/Read compare: "<<evm1->compareEVM(evm2)<<endl;
}

/**
 * Test para la operacion union de objetos OLAF
 */
void testUnion(){
    // Pruebas para verificar la secuencia de secciones y de couplets
    
    nDEVM<int> *evm1 = new nDEVM<int>();
    string fileName1 = "rawFiles/olaf.raw";
    evm1->rawFileToEVM(fileName1,128,128,128);
    
    nDEVM<int> *evm2 = new nDEVM<int>();
    string fileName2 = "rawFiles/olaf45.raw";
    evm2->rawFileToEVM(fileName2,128,128,128);
    
    nDEVM<int>* evmOp = evm1->booleanOperation(evm2,"union");
    
    nDEVM<int>* evm3 = evmOp->EVMCoupletSequence();
    cout<<endl<<"Couplet Sequence obtained..."<<endl;
    //evm3->printTrie();
}

/**
 * Cargar una secuencia de frames como una animacion...
 */
void testAnimationLoad(){
    nDEVM<unsigned int> *evm = new nDEVM<unsigned int>();
    evm->generateAnimation("Sequences/Pasillo/frame0",780,880);
    evm->frameSequence(780,880);
    return;
}

void testLoadEVMSeq(){
    for(int i = 1590; i <= 1600; i++){
        nDEVM<unsigned int> *frame = new nDEVM<unsigned int>();
        frame->readEVM("frame"+to_string(i));
        frame->EVMFile("frame",i);
        delete frame;
    }
}

void testImageLoad(){
    nDEVM<double> *evm = new nDEVM<double>();
    evm->loadImageFile("Sequences/JackJack/1673.bmp");
    evm->EVMFile("frame",0);
}

void testSequences(){
    // Pruebas para verificar la secuencia de secciones y de couplets
    
    nDEVM<int> *evm1 = new nDEVM<int>();
    string fileName2 = "rawFiles/VL-vismale-(128x256x256)-(1.5,1,1).raw";
    evm1->rawFileToEVM(fileName2,128,256,256);
    
    nDEVM<int>* evm2 = evm1->EVMSectionSequence();

    cout<<endl<<"Section Sequence Obtained..."<<endl;
    //evm2->printTrie();
    
    nDEVM<int>* evm3 = evm2->EVMCoupletSequence();
    cout<<endl<<"Couplet Sequence obtained..."<<endl;
    //evm3->printTrie();
    
    cout<<"Comparacion del EVM de la secuencia de couplets y el EVM original: "
            <<evm1->compareByCouplets(evm3)<<endl;    
}

void testOperations(){
    // Test de Operaciones Booleanas
    char x;
    cout<<"Run? (y|n):";
    cin>>x;
    if(x == 'y'){
        for(int dim = 3; dim <= 3; dim++){
            test("xor",dim,10,100);
            cout<<"Test "+to_string(dim)+"D Done...\n";
        }
    }

    // -- Content Tests

//    nDEVM<double> *loadedFile = new nDEVM<double>();
//    loadedFile->loadnDRawFile("Tests/3DTest/hv2File0.raw",10,3);
//    cout<<"Contenido: "<<loadedFile->content()<<"\n";
}

void test(string operation, int dim,int voxelSize,int iterations){
    srand( time( NULL ) );
    //int dim = 2;
    // Archivo donde se guarda el tiempo de ejecución...
    ofstream executionTime("Tests/"+to_string(dim)+"DTest/executionTime.txt",ios_base::out | ios_base::app);
    if (!executionTime.is_open()){
        cerr << "Can't open Tests/"+to_string(dim)+"DTest/executionTime.txt file for output.\n";
        return;
    }
    
    for(int i = 0; i < iterations; i++){
        if(!test(voxelSize,dim,operation,i,&executionTime)){
            cout<<"ERROR!!!"<<endl;
            break;
        }
        //cout<<"----------\n";
    }
    executionTime.close();
    
}

/**
 * Pruebas de operaciones booleanas para n dimensiones...
 * @param size: El tamaño de las hyper-voxelizaciones.
 * @param dim: La dimensión del espacio.
 * @param testIndex: Índice con el que serán guardados los archivos binarios.
 * @return 
 */
bool test(int size,int dim,string operation, int testIndex,ofstream *executionTime){
    nDEVM<double> *hvEVM1,*hvEVM2,*hvResult,*evmResult;
    bool compare;
    double *voxelInput = new double [dim];
    
    hvEVM1 = new nDEVM<double>();
    hvEVM2 = new nDEVM<double>();
    hvResult = new nDEVM<double>();
    operation[0] = toupper(operation[0]);
    ofstream hv1File("Tests/"+to_string(dim)+"DTest/hv1File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hv2File("Tests/"+to_string(dim)+"DTest/hv2File"+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
    ofstream hvResultFile("Tests/"+to_string(dim)+"DTest/hv"+operation+to_string(testIndex)+".raw",ios_base::out | ios_base::binary);
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
    
    operation[0] = tolower(operation[0]);    
    hvGeneration(operation,&voxelInput,size,&hvEVM1,&hvEVM2,&hvResult,&hv1File,&hv2File,&hvResultFile,dim,dim);

    hv1File.close();
    hv2File.close();
    hvResultFile.close();

    timestamp_t t0 = get_timestamp();
    evmResult = hvEVM1->booleanOperation(hvEVM2,operation);

    timestamp_t t1 = get_timestamp();
    double secs = (t1 - t0) / 1000000.0L;
    *executionTime<<hvEVM1->size()<<"\t"<<hvEVM2->size()<<"\t"<<secs<<endl;

    compare = evmResult->compareEVM(hvResult);
    //hvEVM1->content();
    delete hvEVM1;
    delete hvEVM2;
    delete evmResult;
    delete hvResult;
    delete voxelInput;

    return compare;

//    if(compare){
//        cout<<"Comparacion de operacion "+operation+"; HyperVoxelizaciones VS EVMs: True"<<endl;
//
//        nDEVM<double> *loadedFile = new nDEVM<double>();
//        loadedFile->loadnDRawFile("Tests/"+to_string(dim)+"DTest/hv"+operation+to_string(testIndex)+".raw",size,dim);
//        cout<<"Comparación del EVM formado con el archivo binario resultante y el EVM resultante:  "<<loadedFile->compareEVM(hvResult)<<endl;
//        return true;
//    }else{
//        hvEVM1->setCoord(0);
//        hvEVM1->EVMFile(3001);
//        hvEVM2->setCoord(0);
//        hvEVM2->EVMFile(3002);
//        hvResult->setCoord(0);
//        hvResult->EVMFile(3000);
//        evmResult->setCoord(0);
//        evmResult->EVMFile(3003);
//        cout<<"Comparacion de operacion Union; HyperVoxelizaciones VS EVMs: False"<<endl;
//        return false;
//    }
}

void hvGeneration(string operation,double **voxelInput,int size,nDEVM<double> **hvEVM1,nDEVM<double> **hvEVM2, nDEVM<double> **hvResult,
        ofstream *hv1File,ofstream *hv2File,ofstream *hvResultFile, int dim,int currentDim){
    
    if(currentDim == 0){
        unsigned char value1, value2,result;    // Las hypervoxelizaciones tienen dimensiones menores a 8 bits...
        //bool hyperBox1,hyperBox2;
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

        if(booleanOperation(operation,value1,value2)){
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
        hvGeneration(operation,voxelInput,size,hvEVM1,hvEVM2,hvResult,hv1File,hv2File,hvResultFile,dim,currentDim-1);
    }
}

bool booleanOperation(string op,unsigned char value1, unsigned char value2){
    bool hyperBox1,hyperBox2;
    // Si la hyperBox esta llena o vacia
    hyperBox1 = value1 == 1;
    hyperBox2 = value2 == 1;

    if(op.compare("union") == 0){
        return hyperBox1 or hyperBox2;
    }
    
    if(op.compare("intersection") == 0){
        return hyperBox1 and hyperBox2;
    }

    if(op.compare("difference") == 0){
        return (value1 - value2) > 0;
    }

    if(op.compare("xor") == 0){
        return hyperBox1 xor hyperBox2;
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
