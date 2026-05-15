#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h> 
#include "cryptpng.h"

void readBytes(unsigned char** buffer, int count, size_t* bytes_read, int* loop, FILE *f){
  unsigned char *temp = realloc(*buffer, count);
  if(!temp){
    printf("buffer is null, exiting\n");
    free(*buffer);
    exit(3); 
  }
  *buffer = temp;
  *bytes_read = fread(*buffer, 1, count, f);
  if(*bytes_read < count){
    printf("EOF reached\n");
    *loop = 0;
  }
  else if(*bytes_read != count){
    printf("Error reading bytes, exiting\n");
    exit(4);
  } 
}
unsigned char *readInput(){
  int count = 0, size=500, extra = 0, c;
  unsigned char *buffer = malloc(size*sizeof(unsigned char));
  if(buffer == NULL){
    printf("Buffer is null exit 1\n");
    exit(1);
  }
  if(fgets((char*)buffer, size*sizeof(unsigned char), stdin) == NULL){//fgets requires a char*, temporary casting
    printf("failed to read input exit 2\n");
    exit(2);
  }
    return buffer;
}

//function for en/decrypting PNG data with LFSR
void lfsrpng(unsigned char *data, unsigned int initial_value, uint32_t data_length){
  unsigned int feedback = 0x87654321;//define this globally later
  unsigned int lfsr = initial_value;
  for(int i = 0; i < data_length; i++){
    for(int j = 0; j < 8; j++){
      if(lfsr & 1)
        lfsr = (lfsr>>1)^feedback;
      else
        lfsr >>=1;
    }//end for j
    data[i] ^= (lfsr & 0x00FF);
  }//end for i
}

void decompressidat(){
  //consider writing DEFLATE myself
}

void compressidat(){
}

//function for en/decrypting pngs
void cryptpng(){
  //add in user input for file here
  int loop = 1;
  FILE *f = fopen("testfile.png", "rb");//TODO add user input for file names
  FILE *f1 = fopen("output.png", "wb");
  unsigned char png_signature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
  // unsigned char idat_hdr[4] = {0x49, 0x44, 0x41, 0x54};//possible endian issue?
  uint32_t chunk_length = 0, width = 0, height = 0, scanline_bytes = 0, uncompressed_size = 0;
  uint8_t bit_depth = 0, color_type = 0, compression_method = 0, filter_method = 0, interlace_method = 0, bytes_per_pixel = 0;
  unsigned int initial_value = 0x12345678; // Initial value for LFSR define globally later

  //check if files opened successfully
  if(!f || !f1){
    printf("failed to open file, exiting\n");
    exit(1);
  }

  //check if PNG(or looks like PNG)
  unsigned char *buffer = malloc(8); 
  if(!buffer){
    printf("buffer is null, exiting\n");
    exit(1);
  }
  size_t bytes_read = fread(buffer, 1, 8, f);
  if(bytes_read != 8){
    printf("failed to read png signature, exiting\n");
    exit(1);
  }
  if(memcmp(buffer, png_signature, 8)){
    printf("file is not a png, exiting\n");
    exit(2);
  }


  //get file specification data from IHDR
  readBytes(&buffer, 4, &bytes_read, &loop, f);
  width = ((uint32_t)(unsigned char)buffer[0] << 24) | 
          ((uint32_t)(unsigned char)buffer[1] << 16) | 
          ((uint32_t)(unsigned char)buffer[2] << 8) | 
          (unsigned char)buffer[3];
  readBytes(&buffer, 4, &bytes_read, &loop, f);
  height = ((uint32_t)(unsigned char)buffer[0] << 24) | 
           ((uint32_t)(unsigned char)buffer[1] << 16) | 
           ((uint32_t)(unsigned char)buffer[2] << 8) | 
           (unsigned char)buffer[3];
  readBytes(&buffer, 1, &bytes_read, &loop, f);
  bit_depth = buffer[0];
  readBytes(&buffer, 1, &bytes_read, &loop, f);
  color_type = buffer[0];
  readBytes(&buffer, 1, &bytes_read, &loop, f);
  compression_method = buffer[0];
  readBytes(&buffer, 1, &bytes_read, &loop, f);
  filter_method = buffer[0];
  readBytes(&buffer, 1, &bytes_read, &loop, f);
  interlace_method = buffer[0];

  //##CHECK ZLIB DOCUMENTATION
  //below are possibly required for compression/decompression 
  /*
  samples per pixel is dictated by color type
  0 (grayscale) → 1
2 (truecolor RGB) → 3
3 (indexed/palette) → 1 (palette index only)
4 (grayscale + alpha) → 2
6 (truecolor + alpha RGBA) → 4
  */
  switch(color_type){
    case 0: //grayscale
      samples_per_pixel = 1;
      break;
    case 2: //truecolor
      samples_per_pixel = 3;
      break;
    case 3: //indexed color
      samples_per_pixel = 1;
      break;
    case 4: //grayscale with alpha
      samples_per_pixel = 2;
      break;
    case 6: //truecolor with alpha
      samples_per_pixel = 4;
      break;
    default:
      printf("unsupported color type, exiting\n");
      exit(3);
  }
  bytes_per_pixel = (bit_depth * samples_per_pixel + 7) / 8;
  scanline_bytes = (width * samples_per_pixel * bit_depth + 7) / 8;
  uncompressed_size = height * (1+scanline_bytes); //account for filter byte at start of each scanline
  //## END CHECK

  while(loop){
  //get chunk length
  readBytes(&buffer, 4, &bytes_read, &loop, f);
  fwrite(buffer, 1, 4, f1);
  //chunk length is big endian in PNGs
  chunk_length = ((uint32_t)(unsigned char)buffer[0] << 24) | 
               ((uint32_t)(unsigned char)buffer[1] << 16) | 
               ((uint32_t)(unsigned char)buffer[2] << 8) | 
               (unsigned char)buffer[3];
  readBytes(&buffer, 4, &bytes_read, &loop, f);
  fwrite(buffer, 1, 4, f1);
  if(!memcmp(buffer, idat_hdr, 4)){
    printf("IDAT found\n");
    //fseek in f1 to the current position in f1
    readBytes(&buffer, chunk_length, &bytes_read, &loop, f);//grab chunk data here
    //run decompression here
    lfsrpng(buffer, initial_value, chunk_length);//lfsr on chunk data
    //run compression here
    //compute CRC here
    fseek(f, 4, SEEK_CUR);//move past header(do i still need this?)
    fwrite(buffer, 1, chunk_length, f1);//write encrypted data back to file(note, needs to also write CRC)
  } else {
    printf("IDAT not found, skipping chunk\n");
    readBytes(&buffer, chunk_length+4, &bytes_read, &loop, f);//skip chunk + 4(CRC)
    fwrite(buffer, 1, chunk_length+4, f1); //write chunk + CRC to output file
  }
}//end while
free(buffer);
  fclose(f);
  fclose(f1);
}//end cryptpng