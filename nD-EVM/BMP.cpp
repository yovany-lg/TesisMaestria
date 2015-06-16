/* 
 * File:   BMP.cpp
 * Author: yova
 * 
 * Created on 5 de enero de 2015, 12:34 PM
 */

#include "BMP.h"
#include <iostream>
#include <cstdlib>

/**
 * Constructor para generar una imagen nueva.
 * @param width => Ancho de la imagen
 * @param height => Alto de la imagen
 * @param colors => Cantidad de componentes de color, se considera 1 BYTE por componente
 */
BMP::BMP(int width,int height, int colorCount){
    // Bitmap file header: 14 bytes
    header.identifier = 0x4D42;
    header.headerSize = 40;

    header.bitsPerPixel = colorCount*8;
    // - Paleta de colores obligatoria para menor igual a 8 bits...
    if(header.bitsPerPixel == 8){
        // Sino se sabe cuantos colores son...
//        if(header.numberOfColours == 0)
        header.numberOfColours = 256;

        // - Generar paleta de colores
        pPalette = new Color[header.numberOfColours];
        for(int i = 0; i < header.numberOfColours; i++){
            pPalette[i].a = i;
            pPalette[i].b = i;
            pPalette[i].g = i;
            pPalette[i].r = i;
        }
        header.bitoffset = header.headerSize + 14 + header.numberOfColours * 4;
    }else{
        header.numberOfColours = 0;
        header.bitoffset = header.headerSize + 14;
    }
    
    header.width = width;
    header.height = height;
    header.planes = 1;
    
    header.imageSize = header.width * header.height * colorCount;
    header.size = header.imageSize + header.bitoffset;
    header.compression = 0;
    header.reserved = 0;
    header.hresolution = 0;
    header.vresolution = 0;
    header.importantColours = 0;
    isImageLoaded = true;
}

BMP::BMP(const char *fileName) {
    if(!fileName) return;

    // Reinicio de las variables
    isImageLoaded = false;
    pPalette = NULL;
    pImageData = NULL;

    // Apertura del archivo
    FILE *pFile = fopen(fileName, "rb");	

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
//        if(header.bitsPerPixel != 8){
//            fclose(pFile);
//            return;
//        }

        // Sino se sabe cuantos colores son...
        if(header.numberOfColours == 0)
            header.numberOfColours = (int)pow(2.0, 1.0 * header.bitsPerPixel);

        // Creo y leo la paleta de colores
        pPalette = new Color[header.numberOfColours];
        fread(pPalette, header.numberOfColours, sizeof(Color), pFile);
        
        // - Se calcula el tamano en bytes de la imagen si no existe
        if(header.imageSize == 0){
            header.imageSize = header.width * header.height * (header.bitsPerPixel/8);
        }
        
        // -- Read RGB Image Data
        pImageData = new BYTE[header.imageSize];
        
        // - move file point to the begging of bitmap data
        fseek(pFile, header.bitoffset, SEEK_SET);

        int row_padded = (header.width*3 + 3) & (~3);
        BYTE* data = new BYTE[row_padded];
        int pixelIndex = 0;

        for(int i = 0; i < header.height; i++)
        {
            fread(data, sizeof(BYTE), row_padded, pFile);
            for(int j = 0; j < header.width*3; j += 3)
            {
                // Convert (B, G, R) to (R, G, B)
                pImageData[pixelIndex] = data[j+2];
                pImageData[pixelIndex+1] = data[j+1];
                pImageData[pixelIndex+2] = data[j];

//                cout <<"R: "<<(int)data[j] <<",G: "<<(int)data[j+1]<<",B: "<<(int)data[j+2]<<endl;
                //Validar si es un pixel vacío o relleno
//                if(j < 350){
//                    if(pImageData[pixelIndex] > 0 and pImageData[pixelIndex+1] > 0 and pImageData[pixelIndex+2] > 0)
//                        cout<<' ';
//                    else
//                        cout<<'*';
//                }
                pixelIndex+=3;
            }
//            cout<<endl;
        }
        delete data;

        // - Leer el contenido del BitMap
//        fread(pImageData, 1, header.imageSize, pFile);
        
        //make sure bitmap image data was read
        if (pImageData == NULL){
            fclose(pFile);
            return;
        }
        isImageLoaded = true;

        fclose(pFile);
        
        // - Para verificar contenido
        printHeader();
//        for(int i = 0; i < 1000; i++){
//            cout<<pImageData[i];
//        }
//        cout<<endl<<endl<<pImageData[header.imageSize-3];
    }else{
        cout<<"El archivo "<<fileName<<" no pudo leerse"<<endl;
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
//        printHeader();
    }else{
        cout<<"No se pudo abrir el archivo "<< name<<endl;
        exit(EXIT_FAILURE);
    }
}

BMP::BMP(const BMP& orig) {
}

BMP::~BMP() {
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
                
                // - La paleta de colores es obligatoria cuando los bits por pixel son menores o iguales a 8,
                // - Segun wiki
                if(header.bitsPerPixel <= 8)
                    fwrite(pPalette, sizeof(Color), header.numberOfColours, pFile);

		fseek(pFile, header.bitoffset, SEEK_SET);
                
		fwrite(pImageData, header.imageSize, 1, pFile);
                
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
        delete [] rowData;
    }else{
        cout<<"Nothing Saved..."<<endl;
    }
}

BYTE* BMP::getRGBData(){
    BYTE tempRGB;
//    FILE *pFile = fopen(filename.c_str(), "rb");
    
    if(isImageLoaded){
        //swap the r and b values to get RGB (bitmap is BGR)
        for (int imageIdx = 0; imageIdx < header.imageSize; imageIdx+=3){
            tempRGB = pImageData[imageIdx];
            pImageData[imageIdx] = pImageData[imageIdx + 2];
            pImageData[imageIdx + 2] = tempRGB;
        }

        //close file and return bitmap iamge data
//        fclose(filePtr);
        return pImageData;
    }
}

BYTE* BMP::getImageData(){
    return pImageData;
}

void BMP::printDotImage(string filename){
    BYTE tmp,R,G,B;
    cout<<"Dot Image: "<<endl;
    
//    FILE *pFile = fopen(filename.c_str(), "rb");
//    fseek(pFile, header.bitoffset, SEEK_SET);
//    
//    int row_padded = (header.width*3 + 3) & (~3);
//    unsigned char* data = new unsigned char[row_padded];
//
//    for(int i = 0; i < header.height; i++)
//    {
//        fread(data, sizeof(unsigned char), row_padded, pFile);
//        for(int j = 0; j < header.width*3; j += 3)
//        {
//            // Convert (B, G, R) to (R, G, B)
//            tmp = data[j];
//            data[j] = data[j+2];
//            data[j+2] = tmp;
//
////            cout <<data[j] <<data[j+1]<<data[j+2];
//            //Validar si es un pixel vacío o relleno
//            if(j < 350){
//                if(data[j] > 0 and data[j+1] > 0 and data[j+2] > 0)
//                    cout<<' ';
//                else
//                    cout<<'*';
//            }
//        }
//        cout<<endl;
//    }
//
//    fclose(pFile);
    
    int pixelIndex = 0;

    for(int i = 0; i < header.height; i++)
    {
        for(int j = 0; j < header.width*3; j+= 3)
        {
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
            }
            pixelIndex+=3;
        }
        cout<<endl;
    }
}

void BMP::getPixelRGB(int x,int y, unsigned char **rgb){
    BYTE R,G,B;
    R = pImageData[y*(header.width*3) + x];
    G = pImageData[y*(header.width*3) + x + 1];
    B = pImageData[y*(header.width*3) + x + 2];
    (*rgb)[0] = R;
    (*rgb)[1] = G;
    (*rgb)[2] = B;
}