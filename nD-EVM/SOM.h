/* 
 * File:   SOM.h
 * Author: yova
 *
 * Created on 2 de marzo de 2015, 04:30 AM
 */

#ifndef SOM_H
#define	SOM_H

#include <vector>
#include "DataSet.h"

//typedef double * weights;

class SOM {
public:
    vector<double> patterns;
    double **weightMatrix;
//    DataSet *dataSet;
    int neurons;
    int dimension;
    double sigma,t1;
    double eta,t2;
    int iter;
    
//    SOM(DataSet *_dataSet,int _dim,int _neurons);
    SOM(int _neurons);
    SOM(const SOM& orig);
    virtual ~SOM();
    void initialize();
    void sampling();
    int minDistanceNeuron(double *data);
    double vectorDistance(double *v1,double *v2);
    void weightUpdates(double *data,int winnerKey, int n);
    void weightUpdate(double *data,int currentNeuronKey,int winnerKey,int n);
    double neighborDistance(int excitedNeuronKey,int winnerKey,int n);
    double effectiveWidth(int n);
    double learnRate(int n);
    void dataSetClustering();
    void loadBinFile(string fileName);
    string vectorToString(double *vector,int size);
    void printWeights();
    
    void clustering();
private:

};

#endif	/* SOM_H */

