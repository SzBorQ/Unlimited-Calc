#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"
#include "read.h"



int main(){
    FILE *in, *out;
    
    in = fopen("in.txt", "r");
    out = fopen("out.txt", "w");

   processFile(in,out);
}