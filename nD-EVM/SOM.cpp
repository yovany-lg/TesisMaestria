/* 
 * File:   SOM.cpp
 * Author: yova
 * 
 * Created on 2 de marzo de 2015, 04:31 AM
 */

#include "SOM.h"

SOM::SOM(int _neurons) {
//    dataSet = _dataSet;
    dimension = 1;
    neurons = _neurons;
    iter = 100;
    eta = 0.01;t2 = 500.0;
    sigma = 0.001*neurons;
    t1 = 500.0/log10(sigma);

    weightMatrix = new double*[neurons]; 
    for(int i = 0 ; i<neurons;i++){
        weightMatrix[i] = new double[dimension]; //Considerando el bias en la primera posicion
        for(int j = 0; j<(dimension);j++)
        {
            weightMatrix[i][j] = 0;   //Inicializacion de los pesos y el bias        
        }
    }
}

SOM::SOM(int _neurons,int _parts,int _dcFiles) {
//    dataSet = _dataSet;
    dimension = 1;
    neurons = _neurons;
    iter = 100;
    eta = 0.1;t2 = 100.0;
    sigma = 0.0001*neurons;
    t1 = 100.0/log10(sigma);

    weightMatrix = new double*[neurons]; 
    for(int i = 0 ; i<neurons;i++){
        weightMatrix[i] = new double[dimension]; //Considerando el bias en la primera posicion
        for(int j = 0; j<(dimension);j++)
        {
            weightMatrix[i][j] = 0;   //Inicializacion de los pesos y el bias        
        }
    }
    
    parts = _parts;
    dcFiles = _dcFiles;
}

SOM::SOM(const SOM& orig) {
}

SOM::~SOM() {
}

template <typename T> std::string to_string(T value){
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

void SOM::initialize(){
//    int setSize = dataSet->patterns.size();
//    int setSize = patterns.size();

//    int randKey;
    srand (time(NULL));
    
    float randStep = 1.0/10000; //Pasos discretos
    double randValue;
    
    for(int i = 0; i < neurons; i++){
//        randKey = rand() % setSize;
        for(int j = 0; j < dimension; j++){
            randValue = (rand() % 10000)*randStep;            
//            weightMatrix[i][j] = patterns[randKey];
            weightMatrix[i][j] = randValue;
            cout<<"Init Weight["<<i<<"]: "<<weightMatrix[i][j]<<endl;
        }
    }
}

/**
 * Presentaciones del Data Set
 */
void SOM::sampling(){
    int winner;
    string fileName = "";
    string part = "";
    double *dcValue = new double;

//    dataSize = patterns.size();
    for(int i = 0; i < iter; i++){
//        cout<<"Iteration["<<i<<"]..."<<endl;
        // - Lectura de las partes en que se dividen los archivos de DC
        for(int j = 0; j < parts; j++){
            part = "Part"+to_string(j);
            // - Lectura de los archivos de cada parte
            for(int k = 0; k < dcFiles; k++){
                
                fileName = "dcFiles/"+part+"/dcFile"+to_string(k)+".dc";
                ifstream fileInput;
                fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
                
                if (! fileInput.is_open()){
                    cout<<"El archivo: "<<fileName<<" no pudo abrirse!!!..."<<endl;
                    return;
                }else
//                {
//                    cout<<"Reading File: "<<fileName<<endl;
//                }

                while(fileInput.read((char *) dcValue, sizeof(double))){
                    winner = minDistanceNeuron(dcValue);
                    weightUpdates(dcValue,winner,i);            
//                    cout <<"DC: "<<*dcValue <<endl; 
//                    i++;
                }
                fileInput.close();
                
            }
        }
        
        
//        for(int j = 0; j < dataSize; j++){
//            winner = minDistanceNeuron(&(patterns[j]));
//            weightUpdates(&(patterns[j]),winner,i);            
////            cout<<"Winner: "<<vectorToString(&(patterns[j]),dimension)
////                    <<" => "<< winner<<endl;
//        }
//        cout<<"----------"<<endl;
//        printWeights();
//        return;
    }
    cout<<"----------Final Weights"<<endl;
    printWeights();
    delete dcValue;
}

int SOM::minDistanceNeuron(double *pattern){
    double minDistance = 0.0,currentDistance;
    int minDistanceKey = -1;
    
    for(int i = 0; i < neurons; i++){
        currentDistance = vectorDistance(pattern,weightMatrix[i]);
        if(minDistanceKey == -1)
        {
            minDistanceKey = i;
            minDistance = currentDistance;
        }else
        {
            if(currentDistance < minDistance)
            {
                minDistanceKey = i;
                minDistance = currentDistance;
            }
        }
    }
    
    return minDistanceKey;
}

double SOM::vectorDistance(double *v1,double *v2){
    double diffSum = 0.0;
    double diff;
    for(int i = 0; i < dimension; i++)
    {
        diff = v1[i] - v2[i];
        diffSum += diff*diff;
    }

    return sqrt(diffSum);
}

void SOM::weightUpdates(double *data,int winnerKey, int n){
//    cout<<"Data: "<<data[0]<<endl;
    for(int i = 0; i < neurons; i++){
        weightUpdate(data,i,winnerKey,n);
    }
}

void SOM::weightUpdate(double *data,int currentNeuronKey,int winnerKey,int n){
    double nDistance = neighborDistance(currentNeuronKey,winnerKey,n);
    double lRate = learnRate(n);

    if(lRate < 0.01)
        lRate = 0.01;
    double weightItem,dataItem,weightUpdate;
    for(int i = 0; i < dimension; i++)
    {
        weightItem = weightMatrix[currentNeuronKey][i];
        dataItem = data[i];
        weightUpdate = weightItem+lRate*nDistance*(dataItem-weightItem);
        weightMatrix[currentNeuronKey][i] = weightUpdate;
    }
}

double SOM::neighborDistance(int excitedNeuronKey,int winnerKey,int n){
    double lateralDistance = pow(vectorDistance(weightMatrix[excitedNeuronKey],weightMatrix[winnerKey]),2);
    double effWidth = pow(effectiveWidth(n),2);

    return exp(-(lateralDistance)/(2*effWidth));
}

double SOM::effectiveWidth(int n){
    return sigma*exp(-((double) n)/(t1));
}

double SOM::learnRate(int n){
    return eta*exp(-((double) n)/t2);
}

//void SOM::dataSetClustering(){
//    int dataSize = patterns.size();
//    int winner;
//    cout<<endl<<"SOM Clustering:"<<endl;
//    for(int i = 0; i < dataSize; i++){
//        winner = minDistanceNeuron(&patterns[i]);
//        cout<<patterns[i]<<','<<winner+1<<endl;
//    }
//}

void SOM::clustering(){
    ofstream **files = NULL;
    string fileName = "";
    string part = "";
//    unsigned int dataSize = patterns.size();
    int winner;
    double *dcValue = new double;
    unsigned int i = 0;

    // - Abrir archivos de clustering
    files = new ofstream*[neurons];
    
    for(int i = 0; i < neurons; i++){
        fileName = "clustering/cluster"+to_string(i)+".idx";
        files[i] = new ofstream(fileName.c_str(),ios_base::out|ios_base::binary);

        if ( ! files[i]->is_open() ){    
            cout << "El archivo"<<fileName<<" no se pudo abrir!" << '\n';    
            return;
        } 
        //        (*files)[i]
    }
    
    // - Proceso de Clustering
    cout<<endl<<"SOM Clustering..."<<endl;

    // - Lectura de las partes en que se dividen los archivos de DC
    for(int j = 0; j < parts; j++){
        part = "Part"+to_string(j);
        // - Lectura de los archivos de cada parte
        for(int k = 0; k < dcFiles; k++){

            fileName = "dcFiles/"+part+"/dcFile"+to_string(k)+".dc";
            ifstream fileInput;
            fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file

            if (! fileInput.is_open()){
                cout<<"El archivo: "<<fileName<<" no pudo abrirse!!!..."<<endl;
                return;
            }

            while(fileInput.read((char *) dcValue, sizeof(double))){
                winner = minDistanceNeuron(dcValue);
                files[winner]->write((char *) &i,sizeof(unsigned int));
                i++;
            }
            fileInput.close();

        }
    }
    
//    for(unsigned int i = 0; i < dataSize; i++){
//        winner = minDistanceNeuron(&patterns[i]);
//        
//        files[winner]->write((char *) &i,sizeof(unsigned int));
//        
////        cout<<patterns[i]<<','<<winner+1<<endl;
//    }    
//    cout<<i<<" DC Count...";
    // - Cerrar todos los arhivos de clustering
    for(int i = 0; i < neurons; i++){
        files[i]->close();
        delete files[i];
    }
    delete [] files;
}

//void SOM::clusterContent(int n){
//    string fileName = "clustering/cluster"+to_string(n)+".idx";
//
//    ifstream fileInput;
//    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file    
//
//    if (! fileInput.is_open()){
//        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
//        return;
//    }
//    int i = 0;
//    unsigned int *dcValue = new unsigned int;
//    
//    while(fileInput.read((char *) dcValue, sizeof(unsigned int))){
//        patterns.push_back(*dcValue);
//        cout <<"idx["<<i<<"]: "<<*dcValue <<endl; 
//        i++;
//    }
//    delete dcValue;
//}

void SOM::printWeights(){
    for(int i = 0; i < neurons;i++){
        cout<<"Weight["<<i<<"]: "<<vectorToString(weightMatrix[i],dimension)<<endl;
    }
}

//void SOM::loadBinFile(string fileName){
//    ifstream fileInput;
//    fileInput.open(fileName.c_str(), ios_base::in |ios_base::binary); // binary file
//    double *dcValue = new double;
//    
//    if (! fileInput.is_open()){
//        cout<<"El archivo: "<<fileName<<" no pudo abrirse..."<<endl;
//        return;
//    }
//
//    int i = 0;
//    while(fileInput.read((char *) dcValue, sizeof(double))){
//        patterns.push_back(*dcValue);
////        cout <<"DC["<<i<<"]: "<<*dcValue <<endl; 
//        i++;
//    }
//    fileInput.close();
//    delete dcValue;
//}

string SOM::vectorToString(double *vector,int size){
    string output="(  ";
    for(int i =0;i<size;i++)
        output+=to_string(vector[i])+"  ";
    return output+")";
}
