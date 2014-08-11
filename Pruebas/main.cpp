/* 
 * File:   pruebas.cpp
 * Author: yova
 *
 * Created on 4 de agosto de 2014, 04:15 PM
 */

// binary.cpp -- binary file I/O
#include <iostream> // not required by most systems
#include <fstream>
#include <iomanip>
#include <cstdlib> // for exit()
inline void eatline() { while (std::cin.get() != '\n') continue; }
struct planet
{
    char name[20]; // name of planet
    double population; // its population
    double g; // its acceleration of gravity
};
//const char * file = "planets.dat";

int main()
{
    using namespace std;
    const char * file = "VL-vismale-(128x256x256)-(1.5,1,1).raw";
    ofstream outputFile( "evm.txt" );
    ifstream fin;
    fin.open(file, ios_base::in |ios_base::binary); // binary file
    unsigned char buffer;
    
    if (fin.is_open())
    {
        cout << "Here are the current contents of the "
        << file << " file:\n";
        
        for(int x1 = 0; x1 < 128; x1++)
            for(int x2 = 0; x2 < 256; x2++)
                for(int x3 = 0; x3 < 256; x3++)
                {
                    if(fin.read(reinterpret_cast<char*>( &buffer ), sizeof(buffer)))
                    {
                        double value = buffer;
                        
                    }
                }
        
//        while(fin.read(reinterpret_cast<char*>( &buffer ), sizeof(buffer)))
//        {
//            int value = buffer;
//            
//            cout << value << endl;
//        }
        fin.close();
    }
    return 0;
}