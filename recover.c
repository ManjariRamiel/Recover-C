#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
int main(int argc, char *argv[]){
    // checking: there should be exactly one argument
    if(argc != 2){
        printf("Usage: ./recover card.raw\n");
        return 1;
    }
        // now we should open the file 
        FILE *card = fopen(argv[1], "r");
        if(card == NULL){
            printf("Could not open file. \n");
            return 1;
    }
    uint8_t buffer[512];
    char filename[8];
    int count = 0;
    FILE *img = NULL;

    // now we should read the card by 512 bytes blocks
    while(fread(buffer, 1, 512, card) == 512){
        // the start of the picture
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0){
            // if the the fike is already open, we should close it
            if(img != NULL){
                fclose(img);
            }
            //now we should create a file named 000.jpg
            sprintf(filename, "%03i.jpg", count);
            // open the file for writing
            img = fopen(filename, "w");
            // increase the counter
            count++;
        }
         // if the file is open, to set the current block in this file
        if(img != NULL){
            fwrite(buffer, 1, 512, img);
        }
    }

    // close all files that are open 
    if(img != NULL){
        fclose(img);
    }
    fclose(card);
    return 0;
}