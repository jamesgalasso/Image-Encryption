#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//function for en/decrypting strings
unsigned char *cryptstring(unsigned char *data, unsigned int initialValue){
    unsigned int lfsr = initialValue, feedback = 0x87654321;
    int dataLength = strlen(data);
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
void cryptimage(){
}//end cryptimage

//function for en/decrypting pngs
void cryptpng(){
  //add in user input for file here
  //getting segfaults. not reading in the file correctly
  char c;
  int x = 0;
  FILE *f = fopen("testfile.png", "r");
  //do more research into what the header looks like, seeing a lot of null bytes at the beginning.
  //header appears correctly but it's got a lot of FF. Find out why
  char *buffer = (char*)malloc(8); 
  printf("**header**\n");
  for(int i = 0; i < 8; i++){
    printf("%02X ", c=fgetc(f));
  }

  printf("\n**ihdr**\n");
  //do a hex to binary conversion later to check these
  printf("width: %02X %02X %02X %02X\n", fgetc(f), fgetc(f), fgetc(f), fgetc(f));
  printf("height: %02X %02X %02X %02X\n", fgetc(f), fgetc(f), fgetc(f), fgetc(f));
  printf("\nrest of the file:\n");
  while((c=fgetc(f)) !=EOF){
    printf("%02X ", c);
    if(!(++x % 16)){
      putc('\n',stdout);
    }
  }
  fclose(f);
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
  cryptpng();
  // printf("enter input\n");
  // buffer = readInput();
  // printf("encrypted data: %s\n", cryptstring(buffer, 0x12345678));
  // printf("decrypted data: %s\n", cryptstring(buffer, 0x12345678));
  return 0;
}//end main
