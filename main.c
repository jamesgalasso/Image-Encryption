#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h> 
#include "cryptpng.h"

//function for en/decrypting strings
// unsigned char *cryptstring(unsigned char *data, unsigned int initialValue){
//     unsigned int lfsr = initialValue, feedback = 0x87654321;
//     int dataLength = strlen(data);//find a better approach for this, null characters will prematurely end the string. 
//     for(int i = 0; i < dataLength; i++){//for each element in the array
//       for(int j = 0; j < 8; j++){ //for each bit in each character
// 	        if(lfsr & 1) //if XXXXXXXX & 00000001 is 1
// 	          lfsr = (lfsr>>1)^feedback; //shift right by 1 and XOR with feedback
// 	        else
// 	          lfsr >>=1; //else just shift right by 1
//       }//end for j
//       data[i] ^= (lfsr & 0x00FF);//XOR the character with the last 8 bits of the lfsr
//     }//end for i
//     return data;
//   }//end cryptstring

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
