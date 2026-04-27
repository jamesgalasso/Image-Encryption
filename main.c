#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void readBytes(char* buffer, int count, size_t* bytes_read, int* loop, FILE *f){
  buffer = realloc(buffer, count);
  if(!buffer){
    printf("buffer is null, exiting\n");
    exit(3); 
  }
  *bytes_read = fread(buffer, 1, count, f);
  if(*bytes_read < 4){
    printf("EOF reached\n");
    *loop = 0;
  }
  else if(*bytes_read != 4){
    printf("Error reading bytes, exiting\n");
    exit(4);
  } 
}
char *readInput(){
  int count = 0, size=500, extra = 0, c;
  char *buffer = malloc(size*sizeof(char));
  if(buffer == NULL){
    printf("Buffer is null exit 1\n");
    exit(1);
  }
  if(fgets(buffer, size*sizeof(char), stdin) == NULL){
    printf("failed to read input exit 2\n");
    exit(2);
  }
    return buffer;
}

//function for en/decrypting strings
unsigned char *cryptstring(unsigned char *data, unsigned int initialValue){
    unsigned int lfsr = initialValue, feedback = 0x87654321;
    int dataLength = strlen(data);//find a better approach for this, null characters will prematurely end the string. 
    for(int i = 0; i < dataLength; i++){//for each element in the array
      for(int j = 0; j < 8; j++){ //for each bit in each character
	        if(lfsr & 1) //if XXXXXXXX & 00000001 is 1
	          lfsr = (lfsr>>1)^feedback; //shift right by 1 and XOR with feedback
	        else
	          lfsr >>=1; //else just shift right by 1
      }//end for j
      data[i] ^= (lfsr & 0x00FF);//XOR the character with the last 8 bits of the lfsr
    }//end for i
    return data;
  }//end cryptstring
//function for en/decrypting any image
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

//function for en/decrypting pngs
void cryptpng(){
  //add in user input for file here
  int c;
  int x = 0;
  int loop = 1;
  FILE *f = fopen("testfile.png", "rb");
  unsigned char png_signature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
  unsigned char idat_hdr[4] = {0x49, 0x44, 0x41, 0x54};//possible endian issue?
  uint32_t chunk_length = 0;
  unsigned int initial_value = 0x12345678; // Initial value for LFSR define globally later

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

  
  while(loop){
  //get chunk length
  readBytes(buffer, 4, &bytes_read, &loop, f);
  //chunk length is big endian in PNGs
  chunk_length = ((uint32_t)(unsigned char)buffer[0] << 24) | 
               ((uint32_t)(unsigned char)buffer[1] << 16) | 
               ((uint32_t)(unsigned char)buffer[2] << 8) | 
               (unsigned char)buffer[3];
  readBytes(buffer, 4, &bytes_read, &loop, f);
  if(!memcmp(buffer, idat_hdr, 4)){
    printf("IDAT found\n");
    readBytes(buffer, chunk_length, &bytes_read, &loop, f);//grab chunk data here
    lfsrpng(buffer, initial_value, chunk_length);//lfsr on chunk data
    fseek(f, -chunk_length-4, SEEK_CUR);//CRC works on both IDAT header & data, move back to before the header
    //compute CRC here
    fseek(f, 4, SEEK_CUR);//move past header(rewrite maybe?)
    fwrite(buffer, 1, chunk_length, f);//write encrypted data back to file
    //write computed CRC here
  } else {
    printf("IDAT not found, skipping chunk\n");
    fseek(f, chunk_length + 4, SEEK_CUR);
  }
}//end while

  free(buffer);
  fclose(f);
}//end cryptpng
//pass address of bytes read later and dont need to return it

int main(){
  int option = 0, max=500;
  char *buffer;
  /*do{
    printf("LFSR - usage n:\n0 to quit\n1 for manual input\n2 for read from file\n");
    
    }while(option);*/
  cryptpng();
  // printf("enter input\n");
  // buffer = readInput();
  // printf("encrypted data: %s\n", cryptstring(buffer, 0x12345678));
  // printf("decrypted data: %s\n", cryptstring(buffer, 0x12345678));
  return 0;
}//end main
