#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"
#include "read.h"



int main(int argc,  char **argv){
    FILE *in, *out;
    char *outName;
    int freeOutName = 0;

    if(argc < 2){
        fprintf(stderr, "Brak argumentow wejsciowych\n");
        return 1;
    }
    in = fopen(argv[1], "r");
    if(!in){
        fprintf(stderr, "Nie moge otworzyc pliku %s\n", argv[1]);
        return 1;
    }

    if(argc < 3){
        outName = createOutName(argv[1]);
        freeOutName = 1;
    } else {
        outName = argv[2];
    }

    out = fopen(outName, "w");
    if(!out){
        fprintf(stderr, "Nie moge otworzyc pliku %s\n", outName);
        fclose(in);
        if(freeOutName) free(outName);
        return 1;
    }

    processFile(in, out);
    fclose(in);
    fclose(out);
    if(freeOutName) free(outName);
    return 0;
}