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
void cryptpng(void);

#endif