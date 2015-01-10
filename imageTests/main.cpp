/* 
 * File:   main.cpp
 * Author: yova
 *
 * Created on 4 de enero de 2015, 05:00 PM
 */

#include <stdio.h>		// Entrada y salida estandar
#include "BMP.h"		// Incluímos nuestra clase BMP
//#include <cstring>
#include <string>
#include <iostream>

void readBMP(string filename);

using namespace std;
/*
 * 
 */
int main(int argc, char** argv) {
//    BMP bmp ("images/1663.bmp");
    BMP bmp ("images/Calibri.bmp");
//    BMP bmp ("lenna.bmp");
    bmp.printHeader();
    bmp.printDotImage("images/Calibri.bmp");
    
    //bmp.brightness(100);
    //bmp.save("brightness+.bmp");	
    //bmp.brightness(-100);
    //bmp.save("brightness-.bmp");	
//    bmp.negative();
//    bmp.save("negative.bmp");
//    bmp.fromRGBtoGRAY();
//    bmp.save("images/copy.bmp");	
//    readBMP("images/Calibri.bmp");
    getchar();	// Con esta línea evitaremos que se cierre la consola
    return 0;
}

void readBMP(string filename)
{
    FILE* f = fopen(filename.c_str(), "rb");
    unsigned char header[54];
    
    if(f == NULL)
    {
        cout << "No se pudo abrir el archivo";
        return;
    }

    //Extraer el header con la información del archivo
    fread(header, sizeof(unsigned char), 54, f);
    
    //Obtener largo y ancho de la imagen
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    cout << "  Name: " << filename << endl;
    cout << " Width: " << width << endl;
    cout << "Height: " << height << endl;
    
    //Tamaño de datos a leer considerando el tamaño de la información del color del pixel
    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;
    
    //Recorrer la matriz de pixeles
    for(int i = 0; i < width; i++)
    {           
        //Leer un pixel
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < height*3; j += 3)
        {
            // Convertir de (B, G, R) a (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;
//            cout<<data[j]<<data[j+1]<<data[j+2];
            //Validar si es un pixel vacío o relleno
            if(data[j] > 0 and data[j+1] > 0 and data[j+2] > 0)
                cout<< ' ';
            else
                cout<< '*';
        }
        cout<<endl;
    }
    fclose(f);
}

