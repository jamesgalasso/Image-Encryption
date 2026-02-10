#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//function for en/decrypting strings
unsigned char *cryptstring(unsigned char *data, unsigned int initialValue){
    unsigned int lfsr = initialValue, feedback = 0x87654321;
    int dataLength = strlen(data);
    for(int i = 0; i < dataLength; i++){
      for(int j = 0; j < 8; j++){
	if(lfsr & 1)
	  lfsr = (lfsr>>1)^feedback;
	else
	  lfsr >>=1;
      }//end for j
      data[i] ^= (lfsr & 0x00FF);
    }//end for i
    return data;
  }//end cryptstring
//function for en/decrypting any image
void cryptimage(){
}//end cryptimage

//function for en/decrypting pngs
void cryptpng(){
}//end cryptpng

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
  /*if(buffer[size-1] != '\n'){
    while((c=getchar()!='\n') && (c != EOF))
      extra=1;
    if(extra){
      printf("potential overflow detected exit 3\n");
      exit(3);
    }
  }
  buffer[size-1] = '\0';*/
  //printf("%s\n", buffer);
    return buffer;
}

int main(){
  int option = 0, max=500;
  char *buffer;
  /*do{
    printf("LFSR - usage n:\n0 to quit\n1 for manual input\n2 for read from file\n");
    
    }while(option);*/
  //buffer not printing
  printf("enter input\n");
  buffer = readInput();
  printf("encrypted data: %s\n", cryptstring(buffer, 0x12345678));
  printf("decrypted data: %s\n", cryptstring(buffer, 0x12345678));
  return 0;
}//end main
