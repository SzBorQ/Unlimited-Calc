#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"
#include "read.h"



int main(int argc,  char **argv){
    FILE *in, *out;
    char *outName;
    if(argc < 2){
        printf("Brak argumentow wejsciowych");
        return -1;
    }
    in = fopen(argv[1], "r");
    if(!in){
        printf("Nie moge otworzyc pliku %s", argv[1]);
        return 0;
    }
    
    outName = (argc < 3)  ? createOutName(argv[1]) : argv[2];
    printf("%s", outName);
    out = fopen(outName, "w");
    if(!out){
        printf("Nie moge otworzyc pliku %s", outName);
        return -1;
    }

    processFile(in,out);
    fclose(in);
    fclose(out);
}