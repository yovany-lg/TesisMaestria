/* 
 * File:   SOM.cpp
 * Author: yova
 * 
 * Created on 2 de marzo de 2015, 04:31 AM
 */

#include "SOM.h"

SOM::SOM(DataSet *_dataSet,int _dim,int _neurons) {
    dataSet = _dataSet;
    dimension = _dim;
    neurons = _neurons;
    iter = 400;
    niu = 0.25;t2 = 500.0;
    sigma = 0.12*neurons;
    t1 = 500.0/log10(sigma);

    weightMatrix = new double*[neurons]; 
    for(int i = 0 ; i<neurons;i++)
    {
        weightMatrix[i] = new double[dimension]; //Considerando el bias en la primera posicion
        for(int j = 0; j<(dimension);j++)
        {
            weightMatrix[i][j] = 0;   //Inicializacion de los pesos y el bias        
        }
    }
}

SOM::SOM(const SOM& orig) {
}

SOM::~SOM() {
}


void SOM::initialize(){
//    Random randomNumbers = new Random();
//    List<Integer> keys = new ArrayList<Integer>(dataSet.keySet());
    int setSize = dataSet->patterns.size();
    int randKey;
    srand (time(NULL));
    for(int i = 0; i < neurons; i++)
    {
        randKey = rand() % setSize;
        for(int j = 0; j < dimension; j++){
            weightMatrix[i][j] = dataSet->patterns[randKey]->inputs[j];
        }
    }
}

/**
 * Presentaciones del Data Set
 */
void SOM::sampling(){
    int winner,dataSize;
    dataSize = dataSet->patterns.size();
    for(int i = 0; i < iter; i++){
        
        for(int j = 0; j < dataSize; j++){
            winner = minDistanceNeuron(dataSet->patterns[j]->inputs);
            weightUpdates(dataSet->patterns[j]->inputs,winner,i);            
        }
    }
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
    return niu*exp(-((double) n)/t2);
}

void SOM::dataSetClustering(){
    int dataSize = dataSet->patterns.size();
    int winner;
    cout<<endl<<"SOM Clustering:"<<endl;
    for(int i = 0; i < dataSize; i++){
        winner = minDistanceNeuron(dataSet->patterns[i]->inputs);
        cout<<dataSet->patterns[i]->inputs[2]<<','<<dataSet->patterns[i]->inputs[3]
                <<','<<winner+1<<endl;
    }
}