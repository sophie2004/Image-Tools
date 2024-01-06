#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read BMP header
void readBMPHeader(FILE *file, unsigned char header[54]) {
    fread(header, sizeof(unsigned char), 54, file);
}
// Function to read BMP color table
void readBMPColorTable(FILE *file, unsigned char colorTable[1024]) {
    fread(colorTable, sizeof(unsigned char), 1024, file);
}

// Function to write BMP header and color table
void writeBMPHeader(FILE *fileOut, unsigned char header[54], unsigned char colorTable[1024]) {
    fwrite(header, sizeof(unsigned char), 54, fileOut);
}

// Function to read BMP pixel data
void readBMPPixelData(FILE *file, unsigned char *buf, int dataSize) {
    fread(buf, sizeof(unsigned char), dataSize, file);
}
// Function to read BMP color table
void writeBMPColorTable(FILE *fileOut, unsigned char colorTable[1024]) {
    fwrite(colorTable, sizeof(unsigned char), 1024, fileOut);
}

// Function to write BMP header and color table
void writeBMPPixelData(FILE *fileOut,  unsigned char *buf, int dataSize) {
    fwrite(buf, sizeof(unsigned char), dataSize, fileOut);
}

int main() {

    //images stored at: ../images/imagename.bmp
    char inputFilePath[256];
    char outputFilePath[256];

    printf("Enter the input BMP file path: ");
    scanf("%255s", inputFilePath);

    // Generating output file path by appending "_copy" to the input file name
    char *copySuffix = "_copy";
    char *extension = ".bmp";

    size_t inputPathLength = strlen(inputFilePath);

    // Ensure the input file path ends with ".bmp"
    if (inputPathLength > 4 && strcmp(inputFilePath + inputPathLength - 4, extension) == 0) {
        snprintf(outputFilePath, sizeof(outputFilePath), "%.*s%s%s", (int)(inputPathLength - 4), inputFilePath, copySuffix, extension);
    } else {
        fprintf(stderr, "Invalid input file format. Expected BMP file.\n");
        return EXIT_FAILURE;
    }

    FILE *fileIn = fopen(inputFilePath, "rb");
    FILE *fileOut = fopen(outputFilePath, "wb");

    if (!fileIn || !fileOut) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    unsigned char header[54];
    unsigned char colorTable[1024];
    
    // Read BMP header
    readBMPHeader(fileIn, header);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int bitDepth = *(int*)&header[28];

    int dataSize = width * height;

    if (bitDepth <= 8) {
        // Read color table if bit depth is 8 or less
        readBMPColorTable(fileIn, colorTable);
    }

    // Write BMP header and color table to output file
    writeBMPHeader(fileOut, header, colorTable);

    unsigned char *buf = (unsigned char*)malloc(dataSize);

    if (!buf) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Read pixel data
    readBMPPixelData(fileIn, buf, dataSize);

    if (bitDepth <= 8) {
    	// Read color table if bit depth is 8 or less
        writeBMPColorTable(fileOut, colorTable);
    } 

    // Write pixel data to output file
    writeBMPPixelData(fileOut, buf, dataSize);

    free(buf);

    fclose(fileIn);
    fclose(fileOut);

    fprintf(stdout, "Image Done\n");
}
