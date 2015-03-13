/* 
 * File:   DataSet.h
 * Author: yova
 *
 * Created on 4 de diciembre de 2014, 12:02 AM
 */

#ifndef DATASET_H
#define	DATASET_H

//#include "Pattern.h"
#include <vector>
#include <cstdlib> // rand(), srand() prototypes
#include <ctime> // time() prototype
#include <iostream>
#include <cmath>
#include <fstream>
#include <cstring>
#include <sstream>
#include <string>

struct Pattern{
    double *inputs;
    double *targets;
};

//template <typename T> std::string to_string(T value)
//{
//	std::ostringstream os ;
//	os << value ;
//	return os.str() ;
//}

using namespace std;
// C, original C++
class DataSet {
public:
    // - Vector con los patrones
    vector<Pattern*> patterns;
    int setSize,nInputs,nOutputs;
    int nClases;

    DataSet();
    DataSet(const DataSet& orig);
    DataSet(int inputs,int outputs);
    DataSet(int inputs,int outputs,int classes);
    virtual ~DataSet();
    
    void addPattern(double *inputVector, double *outputVector);
    void addPattern(Pattern *pattern);
    void loadFile(string fileName);
    void normalize();
    string vectorToString(double *vector,int size);
    void printPattern(Pattern *patt);
    void printDataSet();
    
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);
private:

};

#endif	/* DATASET_H */

