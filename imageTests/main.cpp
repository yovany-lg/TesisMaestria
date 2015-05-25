/* 
 * File:   main.cpp
 * Author: yova
 *
 * Created on 15 de mayo de 2015, 06:49 PM
 */

#include <cstdlib>
#include <stdio.h>		// Entrada y salida estandar
#include "BMP.h"		// Incluímos nuestra clase BMP
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{   
    string fileName = "images/lenna.bmp";
    BMP bmp(fileName);
//    bmp.printDotImage();
    bmp.printHeader();
    //bmp.brightness(100);
    //bmp.save("brightness+.bmp");	
    //bmp.brightness(-100);
    //bmp.save("brightness-.bmp");	
    //bmp.negative();
    //bmp.save("negative.bmp");
//    bmp.fromRGBtoGRAY();
//    bmp.saveImage("images/1663_2.bmp");	
    cout<<"Finally..."<<endl;

//    getchar();	// Con esta línea evitaremos que se cierre la consola
    return 0;
}

