/* 
 * File:   DataSet.cpp
 * Author: yova
 * 
 * Created on 4 de diciembre de 2014, 12:02 AM
 */

#include "DataSet.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <complex>
template <typename T> std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

DataSet::DataSet() {
//    patterns = new vector<Pattern>();
    setSize = 0;
    nInputs = 0;
    nOutputs = 0;
}

DataSet::DataSet(int inputs,int outputs) {
//    patterns = new vector<Pattern>();
    setSize = 0;
    nInputs = inputs;
    nOutputs = outputs;
}

DataSet::DataSet(int inputs,int outputs,int classes) {
//    patterns = new vector<Pattern>();
    setSize = 0;
    nInputs = inputs;
    nOutputs = outputs;
    nClases = classes;
}

DataSet::DataSet(const DataSet& orig) {
}

DataSet::~DataSet() {
}

void DataSet::addPattern(double *inputVector, double *outputVector){
    Pattern *newPattern = new Pattern;
    newPattern->inputs = new double[nInputs];
    newPattern->targets = new double[nOutputs];
    
    for(int i = 0; i < nInputs; i++){
        newPattern->inputs[i] = inputVector[i];
    }
    
    for(int i = 0; i < nOutputs; i++){
        newPattern->targets[i] = outputVector[i];
    }
    patterns.push_back(newPattern);
    setSize++;
}

void DataSet::addPattern(Pattern *pattern){
    Pattern *newPattern = new Pattern;
    newPattern->inputs = new double[nInputs];
    newPattern->targets = new double[nOutputs];
    
    for(int i = 0; i < nInputs; i++){
        newPattern->inputs[i] = pattern->inputs[i];
    }
    
    for(int i = 0; i < nOutputs; i++){
        newPattern->targets[i] = pattern->targets[i];
    }
    patterns.push_back(newPattern);
    setSize++;
}

void DataSet::loadFile(string fileName){
    ifstream inFile; // object for handling file input
    inFile.open(fileName.c_str()); // associate inFile with a file

    if (!inFile.is_open()) // failed to open file
    {
        cout << "Could not open the file " << fileName << endl;
        cout << "Program terminating.\n";
        return;
    }

    string line;
    double value;
//    int count = 0;
    while (inFile.good() && getline(inFile, line))
    {
        Pattern *newPattern = new Pattern;
        newPattern->inputs = new double[nInputs];
        newPattern->targets = new double[nOutputs];
        istringstream iss(line);
        
        for(int i = 0; i < nOutputs; i++){
            if(iss.good() && iss>>value){ // - La clase esta en la primera posicion
                newPattern->targets[i] = value;
//                cout<<value<<" ";
            }
        }
        
        for(int i = 0; i < nInputs; i++){
            if(iss.good() && iss>>value){ // - La clase esta en la primera posicion
                newPattern->inputs[i] = value;
//                cout<<value<<" ";
//                count++;
            }
        }

//        while(iss.good() && iss>>value){
//            newPattern->inputs[count] = value;
////            cout<<value<<" ";
//            count++;
//        }
        printPattern(newPattern);
        patterns.push_back(newPattern);
        setSize++;
//        cout<<"\n"<<count<<endl;
//        count = 0;
        // process pair (a,b)
    }
    
    return;    
}

void DataSet::normalize(){
    double mean, stDev, meanDiff;
    // - Por cada variable de entrada
    for(int j = 0; j < nInputs; j++){
        // - Obtener mean y stDev de cada variable, con respecto a todos los patrones

        // - Mean
        mean = 0;
        for(int i = 0; i < setSize; i++){
            mean += patterns[i]->inputs[j];
        }
        mean = mean/setSize;

        // - stDev
        stDev = 0;
        for(int i = 0; i < setSize; i++){
            meanDiff = (patterns[i]->inputs[j] - mean);
            stDev += meanDiff*meanDiff;
        }
        stDev = sqrt(stDev/setSize);
        
        // - Update
        for(int i = 0; i < setSize; i++){
            patterns[i]->inputs[j] = (patterns[i]->inputs[j] - mean)/stDev;
        }
    }
    cout<<"\n Normalization: \n";
    for(int i = 0; i < setSize; i++){
//        cout<<"P"<<i<<":  ";
        printPattern(patterns[i]);
    }
}


void DataSet::printDataSet(){
    cout<<"\n DataSet: \n";
    for(int i = 0; i < setSize; i++){
        cout<<"P"<<i<<":  ";
        printPattern(patterns[i]);
    }
}

void DataSet::printPattern(Pattern *patt){
    string inputs = vectorToString(patt->inputs,nInputs);
    string outputs = vectorToString(patt->targets,nOutputs);
    
    cout<<outputs<<inputs<<endl;
}

string DataSet::vectorToString(double *vector,int size){
    string output="";
    for(int i =0;i<size;i++)
        output+=to_string(vector[i])+" ";
    return output+"";
}

std::vector<std::string> &DataSet::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> DataSet::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}