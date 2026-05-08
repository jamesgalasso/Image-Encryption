// cryptpng.h
#ifndef CRYPTPNG_H
#define CRYPTPNG_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h> 

void readBytes(unsigned char** buffer, int count, size_t* bytes_read, int* loop, FILE *f);
unsigned char *readInput();
void lfsrpng(unsigned char *data, unsigned int initial_value, uint32_t data_length);
void decompressidat(unsigned char** buffer, uint32_t chunk_length);
void compressidat(unsigned char** buffer, uint32_t chunk_length);
//void computecrc(); see how complex this turns out to be
void cryptpng(void);

#endif