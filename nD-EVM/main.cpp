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

#include <limits>
//#include <thread>       // std::numeric_limits

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
void maskTest(int _xLength, int _yLength,int _timeLegth,int timeShift);
void maskFrameComparison();
void contentTest();
void shiftTest();
void dcFiles();
void SOMTests();
void SOMClustering();
void Clustering();
void SOMClusterContent(int cluster);
void animConv(int timeShift);
void animConv2(int timeShift);
void maskAnimInter(int _xLength, int _yLength,int _timeLegth,int timeShift);
void clusterFrame(int cluster, string fileName,int idx);
void frameImageTest();
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
void frameImageTest();
void clusterContentNC(int cluster);
void clusterContentColor(int cluster);
void animationSize();
void clustersSize();
void clusterContentCount();

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
    // - Manejo de Argumentos de entrada para scripts
//   cout << "ArgsCount = " << argc << endl; 
//   for(int i = 0; i < argc; i++) {
//       string func = string(argv[i]);
//      cout << "argv[" << i << "] = " << func << endl; 
//   }
//   return 0;     
    if(argc == 1){
        cout<<"Library description:\n"<<
                "\n-Functions: It is the first argument nd-evm.exe \"[function]\" [Arguments]\n\n"<<
                "  --AnimLoad: Takes a frame sequence and obtains its correspondent nD-EVM representation.\n"<<
                "    The files generated are the couplets of the animation.";
        return 0;
    }
    string func = string(argv[1]);

    if(func.compare("AnimLoad") == 0){
        // -- Cargar videos en el modelo nD-EVM...
//        cout<<"AnimLoad!!";
        testAnimationLoad();
        return 0;
    }
    
    if(func.compare("AnimConvDC") == 0){
        // - Calculo de DC con la mejora de la secuencia de secciones   
        // --- Script para la ejecucion de la convolucion de una mascara con la animacion
        // --- se ejecuta desde consola y el unico argumento es timeShift, el resto se
        // --- extrae del archivo de configuracion. 
        // --- Solo se procesa un desplazamiento (timeShift) a la vez, los desplazamientos como tal
        // --- se llaman desde la aplicacion en JAVA "AnimConvLauncher.jar".
        animConv(atoi(argv[2]));
        return 0;
    }
    
    if(func.compare("AnimConvContent") == 0){
        // - Convolucion con el descriptor de contenido
        animConv2(atoi(argv[2]));
        return 0;
    }    
    
    if(func.compare("Clustering")){
        // --- Proceso de agrupamiento en base al archivo de configuracion 
        // --- Se considera que ya se obtuvieron los valores de DC mediante la convolucion
       Clustering();
       return 0;
    }
    
    if(func.compare("ClusterContent")){
        // --- Script para obtener el contenido de los clusters, solo en archivos binarios
        // --- *.evm
       SOMClusterContent(atoi(argv[2]));
       return 0;
    }

    if(func.compare("ClusterContentNC")){
        // --- Script para obtener el contenido de los clusters, obtiene las versiones 
        // --- sin informacion de colo, es decir solo regiones 3D, por lo que se obtienen
        // --- 3D-EVMs
        clusterContentNC(atoi(argv[2]));
        return 0;
    }


    if(func.compare("ClusterFrame")){
    // --- Script para extraer los frames de un cluster, se leen los archivos *.evm
    // --- binarios de las Secciones 
    //    if(argc != 4){
    //        cout<<"Argumentos invalidos..."<<endl;
    //        cout<<"cluster fileName idx"<<endl;
    //        return 0;
    //    }    
       clusterFrame(atoi(argv[2]),argv[3],atoi(argv[4]));
    }    
    
    // --- Pruebas Basicas de operaciones booleanas
    // Test de Operaciones Booleanas
//    testOperations();
    
    // - Prueba de secuencias de Couplets y Secciones
//    testSequences();
//    testUnion();
    
    // - Test para cargar imagenes
//    testImageLoad();
    
    // - Test para cargar videos
//    frameImageTest();
//    testLoadEVMSeq();

    // - Test Savings
//    testSaving();
    
    
    // - Content Tests
//    contentTest();
    
    // - Shift Tests
//    shiftTest();

    // - DC Files tests
//    dcFiles();
    
    return 0;
}

void clustersSize(){
    int clusters;
    string fileName;
    ifstream configFile; 
    
    fileName = "bin/config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');
        
        if(lineSplit[0] == "#Clustering"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "clusters:"){
                    clusters = atoi(lineSplit[i+1].c_str());
                }
            }
        }

    }
    configFile.close();
    cout<<"--- [C++ => clustersSize]:"<<endl;
    cout<<"Clustering => Clusters: "<<clusters<<endl;

    unsigned int totalSize = 0;
    unsigned int size;
    nDEVM<unsigned int> *clusterEVM;
//    nDEVM<unsigned int> *colorClusterEVM;

    cout<<"clusters"<<"\tEVMSize"<<endl;
    for(int i = 0; i < clusters; i++){
//        colorClusterEVM = new nDEVM<unsigned int>();
        clusterEVM = new nDEVM<unsigned int>();
        if((i+1)%2 == 0){
            cout<<"\\rowcol ";
        }
        cout<<i<<" & ";
        // - Cluster con color
//        colorClusterEVM->readEVM2("clustering/cluster"+to_string(i)+"/colorClusterEVM"+to_string(i));
//        cout<<colorClusterEVM->size()<<"\t";
        // - Lectura del Cluster sin color
        clusterEVM->readEVM2("clustering/cluster"+to_string(i)+"/clusterEVM"+to_string(i));
        size = clusterEVM->size();
        totalSize += size;
        cout<<size<<"\\\\"<<endl;
        
//        delete colorClusterEVM;
        delete clusterEVM;
    }
    cout<<"\\hline\\hline\n";    
    if((clusters+1)%2 == 0){
            cout<<"\\rowcol ";
    }
    cout<<"\\textbf{Total} & "<<totalSize<<"\\\\"<<endl;
    return;
    
}

void animationSize(){
    // --- Lectura del archivo de configuracion...
    int endFrame;
    string fileName;
    ifstream configFile; 
    
    fileName = "bin/config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');
        
        if(lineSplit[0] == "#Frames"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "endFrame:"){
                    endFrame = atoi(lineSplit[i+1].c_str());
                }
            }            
        }
    }
    configFile.close();
    cout<<"--- [C++ => animationSize]:"<<endl;
    cout<<"Frames => endFrame: "<<endFrame<<endl;    

    int endCouplet = endFrame+1;
    unsigned int size = 0;
    nDEVM<unsigned int> *couplet = new nDEVM<unsigned int>();
    for(int i = 0; i <= endCouplet; i++){
//        cout<<"Reading Couplet "<<i<<"..."<<endl;
        couplet->readEVM2("EVMFiles/frameCouplet"+to_string(i));
        size += couplet->size();
        
        delete couplet;
        couplet = new nDEVM<unsigned int>();
    }
    
    cout<<"Animation EVM Size: "<<size<<endl;
    return;
}

void frameImageTest(){
    nDEVM<unsigned int> *imageEVM = new nDEVM<unsigned int>();
    imageEVM->loadImage("Sequences/Faces/frame3.bmp");

    imageEVM->frameToImage2(90,90,3,"Sequences/Faces/frameImage");
    
}

/**
 * Metodo para realizar la convolucion con el enfoque de la secuencia de Secciones
 * @param _xLength
 * @param _yLength
 * @param _timeLegth
 * @param timeShift
 */
void animConv(int timeShift){
    // --- Lectura del archivo de configuracion...
    int endFrame,xLength, yLength, timeLength,width,length,colors;
    string fileName;
    ifstream configFile; 
    
    fileName = "config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');
        
        if(lineSplit[0] == "#Frames"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "endFrame:"){
                    endFrame = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "colors:"){
                    colors = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "width:"){
                    width = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "length:"){
                    length = atoi(lineSplit[i+1].c_str());
                }
            }            
        }

        if(lineSplit[0] == "#Mask"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "xLength:"){
                    xLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "yLength:"){
                    yLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "timeLength:"){
                    timeLength = atoi(lineSplit[i+1].c_str());
                }
            }            
        }
    }
    configFile.close();
    cout<<"--- [C++ => AnimConvDC]:"<<endl;
    cout<<"Frames => endFrame: "<<endFrame<<", colors: "<<colors<<", width: "<<
            width<<", length: "<<length<<endl;

    cout<<"Mask => xLength: "<<xLength<<", yLength: "<<yLength<<", timeLength: "<<
            timeLength<<", timeShift: "<<timeShift<<endl;
    
    nDEVM<unsigned int> *evm = new nDEVM<unsigned int>();
    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();

    mask->maskInit(xLength, yLength, timeLength,colors,1);
    mask->EVMTraslation(0,timeShift);
    
    // - HQVGA (240x160)
    // - QVGA (320x240)
    // - Obtencion del conjunto DCValues => Discrete Compactness
    evm->maskAnimConv2(mask,endFrame,width,length);    
}

/**
 * Metodo para realizar la convolucion con el enfoque de la secuencia de Secciones
 * @param _xLength
 * @param _yLength
 * @param _timeLegth
 * @param timeShift
 */
void animConv2(int timeShift){
    // --- Lectura del archivo de configuracion...
    int endFrame,xLength, yLength, timeLength,width,length,colors;
    string fileName;
    ifstream configFile; 
    
    fileName = "config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');
        
        if(lineSplit[0] == "#Frames"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "endFrame:"){
                    endFrame = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "colors:"){
                    colors = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "width:"){
                    width = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "length:"){
                    length = atoi(lineSplit[i+1].c_str());
                }
            }            
        }

        if(lineSplit[0] == "#Mask"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "xLength:"){
                    xLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "yLength:"){
                    yLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "timeLength:"){
                    timeLength = atoi(lineSplit[i+1].c_str());
                }
            }            
        }
    }
    configFile.close();
    cout<<"--- [C++ => AnimConvContent]:"<<endl;
    cout<<"Frames => endFrame: "<<endFrame<<", colors: "<<colors<<", width: "<<
            width<<", length: "<<length<<endl;

    cout<<"Mask => xLength: "<<xLength<<", yLength: "<<yLength<<", timeLength: "<<
            timeLength<<", timeShift: "<<timeShift<<endl;
    
    nDEVM<unsigned int> *evm = new nDEVM<unsigned int>();
    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();

    mask->maskInit2(xLength, yLength, timeLength,colors,1);
    mask->EVMTraslation(0,timeShift);
    
    // - HQVGA (240x160)
    // - QVGA (320x240)
    // - Descriptor de Contenido
    evm->maskAnimConv3(mask,endFrame,width,length);    
}

/**
 * Cargar una secuencia de frames como una animacion...
 */
void testAnimationLoad(){
    // --- Lectura del archivo de configuracion...
    int endFrame;
    string fileName;
    ifstream configFile; 
    
    fileName = "config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');
        
        if(lineSplit[0] == "#Frames"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "endFrame:"){
                    endFrame = atoi(lineSplit[i+1].c_str());
                }
            }            
        }
    }
    configFile.close();
    cout<<"--- [C++ => AnimLoad] => endFrame: "<<endFrame<<endl;

    nDEVM<unsigned int> *evm = new nDEVM<unsigned int>();
    evm->generateAnimation("..\\Sequences\\frame",endFrame);
//    evm->frameSequence(endFrame+1);
    return;
}

//void SOMClustering(){
//    std::stringstream stream;
//    stream <<"Clustering.exe";
//    system(stream.str().c_str());
//
//    std::stringstream stream2;
//    stream2 <<"java -jar ClusterContentLauncher.jar";
//    system(stream2.str().c_str());
//
//    std::stringstream stream3;
//    stream3 <<"java -jar ClusterFrameLauncher.jar";
//    system(stream3.str().c_str());
//}

/**
 * Genera un frame a partir de una Seccion de un Cluster
 * @param cluster
 * @param sectionFileName
 * @param idx
 */
void clusterFrame(int cluster, string sectionFileName,int idx){
    int width,length;
    string configFileName;
    ifstream configFile; 
    
    configFileName = "config.txt";            
    configFile.open(configFileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<configFileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');
        
        if(lineSplit[0] == "#Frames"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "width:"){
                    width = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "length:"){
                    length = atoi(lineSplit[i+1].c_str());
                }
            }            
        }
    }
    configFile.close();
    cout<<"--- [C++ => ClusterFrame]:"<<endl;    
    cout<<"Cluster: "<<cluster<<", sectionFileName: "<<sectionFileName<<", idx: "<<
            idx<<", frameWidth: "<<width<<", frameLength: "<<length<<endl;
    nDEVM<unsigned int> *evm = new nDEVM<unsigned int>();
    evm->clusterFrame(cluster,sectionFileName,idx,width,length);
    delete evm;
}

//void frameImageTest(){
////    nDEVM<unsigned int> *currentFrame = new nDEVM<unsigned int>();
//    string imageName = "EVMFiles/frameCouplet2";
////    currentFrame->loadImage("Sequences/frame0.bmp");
////    cout<<"Frame to Image..."<<endl;
////    currentFrame->frameToImage(240,160,currentFrame->dimDepth() - 2,imageName);
//
//
//    nDEVM<unsigned int> *couplet = new nDEVM<unsigned int>();
//    couplet->readEVM2(imageName);
////    couplet->loadImage("Sequences/frame1.bmp");
////    couplet2->readEVM2(imageName);
////    couplet->booleanOperation(couplet2,"intersection");
//    couplet->frameToImage(160,120,couplet->dimDepth() - 2,imageName);
//    
////    delete couplet;
//}

/**
 * Agrupamiento de sub-animaciones a traves de su DC
 */
void Clustering(){
    // --- Lectura del archivo de configuracion...
    int endFrame,timeLength,clusters,dcParts,iter;
    string fileName;
    ifstream configFile; 
    
    fileName = "config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');

        if(lineSplit[0] == "#Frames"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "endFrame:"){
                    endFrame = atoi(lineSplit[i+1].c_str());
                }
            }            
        }

        if(lineSplit[0] == "#Mask"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "timeLength:"){
                    timeLength = atoi(lineSplit[i+1].c_str());
                }
            }            
        }
        
        if(lineSplit[0] == "#Clustering"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "clusters:"){
                    clusters = atoi(lineSplit[i+1].c_str());
                }

                if(lineSplit[i] == "iterations:"){
                    iter = atoi(lineSplit[i+1].c_str());
                }
            }
        }
    }
    configFile.close();
    dcParts = endFrame - timeLength +1;
    cout<<"--- [C++ => Clustering] => Clusters: "<<clusters<<", dcParts: "<<dcParts<<
            ", Iterations: "<<iter<<endl;
    
    nDEVM<unsigned int> *evmClustering = new nDEVM<unsigned int>();
    evmClustering->subAnimClustering(clusters,dcParts,iter);
}

/**
 * Obtiene el contenido de un cluster, esta version obtiene el contenido del Cluster
 * en su totalidad, es decir, que contiene toda la informacion de color.
 * @param cluster: Cluster del cual se obtendra el contenido
 */
void SOMClusterContent(int cluster){
    // --- Lectura del archivo de configuracion...
    int endFrame,xLength, yLength, timeLength,width,length,colors;
    string fileName;
    ifstream configFile; 
    
    fileName = "config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');

        if(lineSplit[0] == "#Frames"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "endFrame:"){
                    endFrame = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "colors:"){
                    colors = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "width:"){
                    width = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "length:"){
                    length = atoi(lineSplit[i+1].c_str());
                }
            }            
        }

        if(lineSplit[0] == "#Mask"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "timeLength:"){
                    timeLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "xLength:"){
                    xLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "yLength:"){
                    yLength = atoi(lineSplit[i+1].c_str());
                }
            }            
        }
    }
    configFile.close();    

    cout<<"--- [C++ => ClusterContent] => Cluster: "<<cluster<<", xLength: "<<xLength<<", yLength: "
            <<yLength<<", timeLength: "<<timeLength<<endl;
    
    nDEVM<unsigned int> *evmClustering = new nDEVM<unsigned int>();

    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();
    mask->maskInit(xLength,yLength,timeLength,colors,1);
    
    evmClustering->clusterContent3(cluster,mask,endFrame,width,length);

    delete mask;
    delete evmClustering;
}

/**
 * Obtener el contenido del Cluster dado, en esta version solo se obtienen las regiones
 * sin considerar la informacion de color, por lo que se obtienen 3D-EVMs.
 * @param cluster: Cluster del cual se obtendra el contenido
 */
void clusterContentNC(int cluster){
    // --- Lectura del archivo de configuracion...
    int endFrame,xLength, yLength, timeLength,width,length,colors;
    string fileName;
    ifstream configFile; 
    
    fileName = "config.txt";            
//    fileName = "bin/config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');

        if(lineSplit[0] == "#Frames"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "endFrame:"){
                    endFrame = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "colors:"){
                    colors = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "width:"){
                    width = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "length:"){
                    length = atoi(lineSplit[i+1].c_str());
                }
            }            
        }

        if(lineSplit[0] == "#Mask"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "timeLength:"){
                    timeLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "xLength:"){
                    xLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "yLength:"){
                    yLength = atoi(lineSplit[i+1].c_str());
                }
            }            
        }
    }
    configFile.close();    

    cout<<"--- [C++ => ClusterContentNC] => Cluster: "<<cluster<<", xLength: "<<xLength<<", yLength: "
            <<yLength<<", timeLength: "<<timeLength<<endl;
    
    nDEVM<unsigned int> *clusterEVM = new nDEVM<unsigned int>();

    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();
    mask->maskInit(xLength,yLength,timeLength,0,1);
    
    clusterEVM = clusterEVM->clusterContentNC(cluster,mask,endFrame,width,length);
//    evmFileName = "..\\clustering\\cluster"+to_string(cluster)+"\\clusterEVM";
//    evmFileName = "clustering/cluster"+to_string(cluster)+"/clusterEVM";
    clusterEVM->dimAmplification(0,5);
    clusterEVM->saveEVM2("..\\clustering\\cluster"+to_string(cluster)+"\\clusterEVM",cluster);
    clusterEVM->EVMFile("..\\clustering\\cluster"+to_string(cluster),"clusterEVM",cluster);
    
    delete mask;
    delete clusterEVM;
}

void clusterContentCount(){
    int clusters;
    string fileName;
    ifstream configFile; 

    fileName = "bin/config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');
        
        if(lineSplit[0] == "#Clustering"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "clusters:"){
                    clusters = atoi(lineSplit[i+1].c_str());
                }
            }
        }

    }
    configFile.close();
    cout<<"--- [C++ => clusterContentCount]:"<<endl;
    cout<<"Clustering => Clusters: "<<clusters<<endl;

    unsigned int count = 0;
    unsigned int *idx = new unsigned int;
    ifstream fileInput;
    
    cout<<"Cluster\t"<<"dcCount"<<endl;
    for(int i = 0; i < clusters; i++){
        fileName = "clustering/cluster"+to_string(i)+".idx";

        fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
        if (! fileInput.is_open()){
            cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
            return;
        }
        while(fileInput.read((char *) idx, sizeof(unsigned int))){
    //        cout <<"Seq["<<i<<"]: "<<*idx <<endl; 
            count++;
        }
        
        fileInput.close();
        cout<<i<<'\t'<<count<<endl;
        count = 0;
    }
    // ***
    

    
    delete idx;
}

/**
 * Obtener el contenido del Cluster...
 * @param cluster: Cluster del cual se obtendra el contenido
 * @param _xLength: Longitud en x de la mascara
 * @param _yLength: Longitud en y de la mascara
 * @param _timeLegth: Longitud temporal de la mascara
 */
void clusterContentColor(int cluster){
    // --- Lectura del archivo de configuracion...
    int endFrame,xLength, yLength, timeLength,width,length,colors;
    string fileName;
    ifstream configFile; 
    
    fileName = "config.txt";            
//    fileName = "bin/config.txt";            
    configFile.open(fileName.c_str(), ios_base::in);
    if (! configFile.is_open()){
        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
        return;
    }
    
    string line;
    while(getline(configFile, line)){
        vector<string> lineSplit = split(line,' ');

        if(lineSplit[0] == "#Frames"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "endFrame:"){
                    endFrame = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "colors:"){
                    colors = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "width:"){
                    width = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "length:"){
                    length = atoi(lineSplit[i+1].c_str());
                }
            }            
        }

        if(lineSplit[0] == "#Mask"){
            for(int i = 1; i < lineSplit.size(); i++){
                if(lineSplit[i] == "timeLength:"){
                    timeLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "xLength:"){
                    xLength = atoi(lineSplit[i+1].c_str());
                }
                if(lineSplit[i] == "yLength:"){
                    yLength = atoi(lineSplit[i+1].c_str());
                }
            }            
        }
    }
    configFile.close();    

    cout<<"--- [C++ => clusterContentColor] => Cluster: "<<cluster<<", xLength: "<<xLength<<", yLength: "
            <<yLength<<", timeLength: "<<timeLength<<endl;
    
    nDEVM<unsigned int> *clusterEVM = new nDEVM<unsigned int>();

    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();
    nDEVM<unsigned int> *superMask;
    mask->maskInit(xLength,yLength,timeLength,colors,1);
    
    superMask = clusterEVM->clusterContentNC(cluster,mask,endFrame,width,length);
    clusterEVM = clusterEVM->maskIntersection(superMask,0,endFrame+1);
//    clusterEVM = clusterEVM->clusterContentColor(cluster,mask,endFrame,width,length);
//    evmFileName = "..\\clustering\\cluster"+to_string(cluster)+"\\clusterEVM";
//    evmFileName = "clustering/cluster"+to_string(cluster)+"/clusterEVM";
    clusterEVM->saveEVM2("..\\clustering\\cluster"+to_string(cluster)+"\\colorClusterEVM",cluster);
//    clusterEVM->EVMFile("..\\clustering\\cluster"+to_string(cluster),"clusterEVM",cluster);
    
    delete mask;
    delete clusterEVM;
}

/**
 * Inicializacion de la mascara y convolucion con la animacion...
 */
void maskTest(int _xLength, int _yLength,int _timeLegth,int timeShift){
    nDEVM<unsigned int> *animMask =  new nDEVM<unsigned int>();
    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();
//    animMask->dcContent(0,0);

    mask->maskInit(_xLength,_yLength,_timeLegth,1,1);
    mask->EVMTraslation(0,timeShift);

    cout<<"Mask => xLength: "<<_xLength<<", yLength: "<<_yLength<<", timeLength: "<<
            _timeLegth<<", timeShift: "<<timeShift<<endl;

    animMask->maskAnimConv(mask,780,880,240,160);    
}

/**
 * Inicializacion de la mascara y convolucion con la animacion...
 */
void maskAnimInter(int _xLength, int _yLength,int _timeLegth,int timeShift){
    nDEVM<unsigned int> *sectionSeq =  new nDEVM<unsigned int>();
    nDEVM<unsigned int> *mask = new nDEVM<unsigned int>();
    nDEVM<unsigned int> *result = new nDEVM<unsigned int>();
//    animMask->dcContent(0,0);

    mask->maskInit(_xLength,_yLength,_timeLegth,1,1);
    mask->EVMTraslation(0,timeShift);

    cout<<"Mask => xLength: "<<_xLength<<", yLength: "<<_yLength<<", timeLength: "<<
            _timeLegth<<", timeShift: "<<timeShift<<endl;
    
    sectionSeq = sectionSeq->maskAnimSections(mask,55);
    
    result = result->maskIntersection(mask,sectionSeq);
    
    int iSection;
    nDEVM<unsigned int> *couplet;
    while(!result->endEVM()){
        iSection = result->getCoord();
        cout<<"Section idx: "<<iSection<<endl;
        couplet = result->readCouplet();
        couplet->frameToImage(240,160,1,"MaskCouplet"+to_string(iSection));
//        couplet->EVMFile("maskAnimCouplet",iSection);
    }
    result->resetCoupletIndex();

//    animMask->maskAnimConv(mask,780,880,240,160);    
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

void testLoadEVMSeq(){
    for(int i = 1590; i <= 1600; i++){
        nDEVM<unsigned int> *frame = new nDEVM<unsigned int>();
        frame->readEVM("frame"+to_string(i));
        frame->EVMFile("frame",i);
        delete frame;
    }
}

void testImageLoad(){
//    nDEVM<double> *evm = new nDEVM<double>();
//    evm->loadImageFile("Sequences/JackJack/frame1565.bmp");
//    evm->EVMFile("JackJackFrame",0);
    int width = 240, height = 160,colorCount = 1;
    BMP bmp(width, height,colorCount);
    
    bmp.pImageData = new BYTE[width*height*colorCount];
//    bmp.saveImage("Images/1663_2.bmp");	
    
    nDEVM<unsigned int> *frame = new nDEVM<unsigned int>();
    nDEVM<unsigned int> *frameMask = new nDEVM<unsigned int>();
    
    frame->readEVM("frameCouplet0");
    
    frameMask->frameMaskInit(colorCount,1);
    
    nDEVM<unsigned int> *inter;
//    nDEVM<unsigned int> *result = new nDEVM<unsigned int>();
//    nDEVM<unsigned int> *prevResult;
    // - Maximo en x
    int j = 0;
    while(frameMask->maskMax[1] <= height){
        while(frameMask->maskMax[0] <= width){
//            cout<<"xShift: "<<frameMask->maskMax[0]<<", yShift: "<<frameMask->maskMax[1]<<endl;
            inter = frame->booleanOperation(frameMask,"intersection");
//            inter->dimMax(2);
            
            for(int i = 0; i < colorCount; i++){
                bmp.pImageData[j] = inter->dimMax(2+i);
                j++;
            }
            
//            inter->printEVM();
//            return;
//            prevResult = result;
//            result = result->booleanOperation(inter,"union");
//            
//            delete prevResult;
            frameMask->EVMTraslation(0,1);
            delete inter;
        }
        frameMask->maskDimReset(0);
        frameMask->EVMTraslation(1,1);
    }
    bmp.saveImage("Images/Frame0.bmp");
    
//    result->EVMFile("frameMask",0);
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

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}