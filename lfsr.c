#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//function for en/decrypting strings
unsigned char *cryptstring(unsigned char *data, int dataLength; unsigned int initialValue){
    unsigned int lfsr = initialValue, feedback = 0x87654321;
    for(int i = 0; i < dataLength; i++){
      for(j = 0; j < 8; j++){
	if(lfsr & 1)
	  lfsr = (lfsr>>1)^feedback;
	else
	  lfsr >>=1;
      }//end for j
      data[i] ^= (lfsr & 0x00FF);
    }//end for i
  }//end cryptstring

//function for en/decrypting any image
void cryptimage(){
}//end cryptimage

//function for en/decrypting pngs
void cryptpng(){
}//end cryptpng

int main(){
  
  return 0;
}//end main
