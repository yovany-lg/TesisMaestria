/* 
 * File:   BMP.cpp
 * Author: yova
 * 
 * Created on 15 de mayo de 2015, 06:50 PM
 */

#include "BMP.h"
//#include <stdio.h>
//#include <cstdlib>
//#include <string>
#include <iostream>
//
using namespace std;

BMP::BMP(const char *name)
{
	if(!name) return;

	// Reinicio de las variables
	isImageLoaded = false;
	pPalette = NULL;
	pImageData = NULL;

	// Apertura del archivo
	FILE *pFile = fopen(name, "rb");	

	// Si se pudo cargar el archivo
	if(pFile)
	{
		// Cargo la cabecera
		fread(&header.identifier,       1, sizeof(WORD),  pFile);		
		fread(&header.size,             1, sizeof(DWORD), pFile);		
		fread(&header.reserved,         1, sizeof(DWORD), pFile);
		fread(&header.bitoffset,        1, sizeof(DWORD), pFile);		
		fread(&header.headerSize,       1, sizeof(DWORD), pFile);		
		fread(&header.width,            1, sizeof(DWORD), pFile);		
		fread(&header.height,           1, sizeof(DWORD), pFile);		
		fread(&header.planes,           1, sizeof(WORD),  pFile);		
		fread(&header.bitsPerPixel,     1, sizeof(WORD),  pFile);		
		fread(&header.compression,      1, sizeof(DWORD), pFile);		
		fread(&header.imageSize,        1, sizeof(DWORD), pFile);		
		fread(&header.hresolution,      1, sizeof(DWORD), pFile);		
		fread(&header.vresolution,      1, sizeof(DWORD), pFile);		
		fread(&header.numberOfColours,  1, sizeof(DWORD), pFile);		
		fread(&header.importantColours, 1, sizeof(DWORD), pFile);	

		// Si el archivo no es de 8bpp, termino la ejecución
		if(header.bitsPerPixel != 8){
			fclose(pFile);
			return;
		}

		// Sino se sabe cuantos colores son...
		if(header.numberOfColours == 0)
			header.numberOfColours = (int)pow(2.0, 1.0 * header.bitsPerPixel);

		// Creo y leo la paleta de colores
		pPalette = new Color[header.numberOfColours];
		fread(pPalette, header.numberOfColours, sizeof(Color), pFile);

		// Cargo los datos de la imagen
		pImageData = new BYTE[header.width * header.height];
		fseek(pFile, header.bitoffset, SEEK_SET);
		fread(pImageData, 1, header.width * header.height, pFile);

		isImageLoaded = true;		
		
		// Cierro el archivo
		fclose(pFile);
	}
}

BMP::BMP(string name){
    if(name == "") return;

    // Reinicio de las variables
    isImageLoaded = false;
    pPalette = NULL;
    pImageData = NULL;

    // Apertura del archivo
    FILE *pFile = fopen(name.c_str(), "rb");	

    // Si se pudo cargar el archivo
    if(pFile)
    {
        // Cargo la cabecera
        fread(&header.identifier,       1, sizeof(WORD),  pFile);		
        fread(&header.size,             1, sizeof(DWORD), pFile);		
        fread(&header.reserved,         1, sizeof(DWORD), pFile);
        fread(&header.bitoffset,        1, sizeof(DWORD), pFile);		
        fread(&header.headerSize,       1, sizeof(DWORD), pFile);		
        fread(&header.width,            1, sizeof(DWORD), pFile);		
        fread(&header.height,           1, sizeof(DWORD), pFile);		
        fread(&header.planes,           1, sizeof(WORD),  pFile);		
        fread(&header.bitsPerPixel,     1, sizeof(WORD),  pFile);		
        fread(&header.compression,      1, sizeof(DWORD), pFile);		
        fread(&header.imageSize,        1, sizeof(DWORD), pFile);		
        fread(&header.hresolution,      1, sizeof(DWORD), pFile);		
        fread(&header.vresolution,      1, sizeof(DWORD), pFile);		
        fread(&header.numberOfColours,  1, sizeof(DWORD), pFile);		
        fread(&header.importantColours, 1, sizeof(DWORD), pFile);	

        // Si el archivo no es de 8bpp, termino la ejecución
        // - Paleta de colores obligatoria para menor igual a 8 bits...
        if(header.bitsPerPixel <= 8){
            // Sino se sabe cuantos colores son...
            if(header.numberOfColours == 0)
                    header.numberOfColours = (int)pow(2.0, 1.0 * header.bitsPerPixel);

            // Creo y leo la paleta de colores
            pPalette = new Color[header.numberOfColours];
            fread(pPalette, header.numberOfColours, sizeof(Color), pFile);
        }
        
//        if(header.bitsPerPixel == 24)
        readData(pFile);
        
//        if(header.bitsPerPixel == 8 )
        
        // - No siempre funciona por el padding
//        fseek(pFile, header.bitoffset, SEEK_SET);
//        fread(pImageData, 1, header.width * header.height, pFile);

        // Cierro el archivo
        fclose(pFile);
    }
}

void BMP::readData(FILE *pFile){
    BYTE pixelBytes = header.bitsPerPixel/8;

    // - Tamano de la fila en bytes
    unsigned int rowSize = floor( (header.bitsPerPixel * header.width + 31) / 32)*4;
    
    // - Se calcula el tamano en bytes de la imagen si no existe
    if(header.imageSize == 0){
        header.imageSize = header.width * header.height * pixelBytes;
    }

    // - Reserver memoria para la informacion de los pixeles
    pImageData = new BYTE[header.imageSize];

    // - move file point to the begging of bitmap data
    fseek(pFile, header.bitoffset, SEEK_SET);

    // - Row size with padding...
//    int row_padded = (header.width*3 + 3) & (~3);
    
    // - vector para leer la fila...
    BYTE* rowData = new BYTE[rowSize];
    
    int pixelIndex = 0;

    for(int i = 0; i < header.height; i++){
        // - leer fila
        fread(rowData, sizeof(BYTE), rowSize, pFile);
        for(int j = 0; j < header.width * pixelBytes; j++){
            // Convert (B, G, R) to (R, G, B)
//            pImageData[pixelIndex] = rowData[j+2];
//            pImageData[pixelIndex+1] = rowData[j+1];
//            pImageData[pixelIndex+2] = rowData[j];

            pImageData[pixelIndex] = rowData[j];
//            pImageData[pixelIndex+1] = rowData[j+1];
//            pImageData[pixelIndex+2] = rowData[j+2];

            pixelIndex++;
        }
    }
    delete rowData;

    //make sure bitmap image data was read
    if (pImageData == NULL){
        fclose(pFile);
        return;
    }

    isImageLoaded = true;		    
}

void BMP::printDotImage(){
    BYTE tmp,R,G,B;
    cout<<"Dot Image: "<<endl;
    
    for(int i = 0; i < header.height; i++)
    {
        for(int j = 0; j < header.width*3; j+= 3)
        {
//            getPixelRGB(j,i,&rgb);
            // Convert (B, G, R) to (R, G, B)
            R = pImageData[i*(header.width*3) + j];
            G = pImageData[i*(header.width*3) + j + 1];
            B = pImageData[i*(header.width*3) + j + 2];

//            cout <<pImageData[j] <<pImageData[j+1]<<pImageData[j+2];
            if(j < 350){
                if(R > 0 and G > 0 and B > 0)
                    cout<<' ';
                else
                    cout<<'*';
//                if(rgb[0] > 0 and rgb[1] > 0 and rgb[2] > 0)
//                    cout<<' ';
//                else
//                    cout<<'*';
            }
//            pixelIndex+=3;
        }
        cout<<endl;
    }
}

BMP::~BMP(void)
{
	if (pImageData != NULL) delete[] pImageData;
	if (pPalette != NULL) delete[] pPalette;
}

void BMP::printHeader(void)
{
	if(isImageLoaded){
		printf("  ===========================================================\n");
		printf("    Image information:\n");
		printf("  ===========================================================\n");

		printf("    + Identifier: \t\t| Ox%X\n", header.identifier);
		printf("    + File size: \t\t| %d bytes\n", header.size);
		printf("    + Data offset: \t\t| %d bytes\n", header.bitoffset);
		printf("    + Header size: \t\t| %d bytes\n", header.headerSize);
		printf("    + Width: \t\t\t| %dpx\n", header.width);
		printf("    + Height: \t\t\t| %dpx\n", header.height);
		printf("    + Planes: \t\t\t| %d\n", header.planes);
		printf("    + Bits per pixel: \t\t| %d-bits\n", header.bitsPerPixel);
		printf("    + Compression: \t\t| %d\n", header.compression);
		printf("    + Image size: \t\t| %d bytes\n", header.imageSize);
		printf("    + Horizontal resolution: \t| %d\n", header.hresolution);
		printf("    + Vertical resolution: \t| %d\n", header.vresolution);
		printf("    + Number of colours: \t| %d\n", header.numberOfColours);
		printf("    + Important colours: \t| %d\n", header.importantColours);
		printf("  -----------------------------------------------------------\n");
	}else{
		printf("\n  + There is not information to show.\n");
	}
}

void BMP::save(const char *name)
{
	if(isImageLoaded)
	{
		if(!name) return;

		FILE *pFile = fopen(name, "wb");

		fwrite(&header.identifier,       sizeof(header.identifier), 1, pFile);
		fwrite(&header.size,             sizeof(header.size), 1, pFile);
		fwrite(&header.reserved,         sizeof(header.reserved), 1, pFile);
		fwrite(&header.bitoffset,        sizeof(header.bitoffset), 1, pFile);
		fwrite(&header.headerSize,       sizeof(header.headerSize), 1, pFile);
		fwrite(&header.width,            sizeof(header.width), 1, pFile);
		fwrite(&header.height,           sizeof(header.height), 1, pFile);
		fwrite(&header.planes,           sizeof(header.planes), 1, pFile);
		fwrite(&header.bitsPerPixel,     sizeof(header.bitsPerPixel), 1, pFile);
		fwrite(&header.compression,      sizeof(header.compression), 1, pFile);
		fwrite(&header.imageSize,        sizeof(header.imageSize), 1, pFile);
		fwrite(&header.hresolution,      sizeof(header.hresolution), 1, pFile);
		fwrite(&header.vresolution,      sizeof(header.vresolution), 1, pFile);
		fwrite(&header.numberOfColours,  sizeof(header.numberOfColours), 1, pFile);
		fwrite(&header.importantColours, sizeof(header.importantColours), 1, pFile);

		fwrite(pPalette, sizeof(Color), header.numberOfColours, pFile);

		fseek(pFile, header.bitoffset, SEEK_SET);
		fwrite(pImageData, 1, header.width * header.height, pFile);

		fclose(pFile);		
	}
}

/**
 * Se guarda una imagen en base a la configuracion del Header actual, y el contenido
 * del pImageData...
 * @param fileName
 */
void BMP::saveImage(string fileName){
    if(isImageLoaded)
    {
//            if(!name) return;
        FILE *pFile = fopen(fileName.c_str(), "wb");

        fwrite(&header.identifier,       sizeof(header.identifier), 1, pFile);
        fwrite(&header.size,             sizeof(header.size), 1, pFile);
        fwrite(&header.reserved,         sizeof(header.reserved), 1, pFile);
        fwrite(&header.bitoffset,        sizeof(header.bitoffset), 1, pFile);
        fwrite(&header.headerSize,       sizeof(header.headerSize), 1, pFile);
        fwrite(&header.width,            sizeof(header.width), 1, pFile);
        fwrite(&header.height,           sizeof(header.height), 1, pFile);
        fwrite(&header.planes,           sizeof(header.planes), 1, pFile);
        fwrite(&header.bitsPerPixel,     sizeof(header.bitsPerPixel), 1, pFile);
        fwrite(&header.compression,      sizeof(header.compression), 1, pFile);
        fwrite(&header.imageSize,        sizeof(header.imageSize), 1, pFile);
        fwrite(&header.hresolution,      sizeof(header.hresolution), 1, pFile);
        fwrite(&header.vresolution,      sizeof(header.vresolution), 1, pFile);
        fwrite(&header.numberOfColours,  sizeof(header.numberOfColours), 1, pFile);
        fwrite(&header.importantColours, sizeof(header.importantColours), 1, pFile);

        if(header.bitsPerPixel <= 8){
            fwrite(pPalette, sizeof(Color), header.numberOfColours, pFile);
        }            

        fseek(pFile, header.bitoffset, SEEK_SET);

        // - Tamano de la fila en bytes
        unsigned int rowSize = floor( (header.bitsPerPixel * header.width + 31) / 32)*4;
        unsigned int pixelIndex = 0;
        // - vector para leer la fila...
        BYTE* rowData = new BYTE[rowSize];
        BYTE pixelBytes = header.bitsPerPixel/8;

        
        for(int i = 0; i < header.height; i++){
            for(int j = 0; j < header.width * pixelBytes; j++){
                rowData[j] = pImageData[pixelIndex];
                pixelIndex++;
            }            
            fwrite(rowData, 1, rowSize, pFile);
        }

        fclose(pFile);  	
    }
}

void BMP::brightness(const int u)
{
	if(u == 0 && !isImageLoaded) return;

	for(int i = 0; i < (int)header.numberOfColours; i++){		
		pPalette[i].r = CLAMP(pPalette[i].r + u);
		pPalette[i].g = CLAMP(pPalette[i].g + u);
		pPalette[i].b = CLAMP(pPalette[i].b + u);
	}
}

void BMP::negative(void)
{
	if(!isImageLoaded) return;

	int n = (int)header.numberOfColours - 1;

	for(int i = 0; i < (int)header.numberOfColours; i++){
		pPalette[i].r = (n - (int)pPalette[i].r);
		pPalette[i].g = (n - (int)pPalette[i].g);
		pPalette[i].b = (n - (int)pPalette[i].b);
	}
}

void BMP::fromRGBtoGRAY(void)
{
	if(!isImageLoaded) return;

	BYTE intensity = 0;
	for(int i = 0; i < (int)header.numberOfColours; i++){
		intensity = RGB2GRAY(pPalette[i].r, pPalette[i].g, pPalette[i].b);
		pPalette[i].r = intensity;
		pPalette[i].g = intensity;
		pPalette[i].b = intensity;
	}
}

