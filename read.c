#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"

#define MAX_L 1024



int checkBase(char *a, int base){
    char z;
    for(int i = 0; i < strlen(a); i++){
        z = a[i];
        if(!((z >= '0' && z <= '9') || (z >= 'A' && z <= 'F'))) return 0;
        if(getValue(z) >= base) return 0;
    }
    return 1;
}

int getHeader(FILE *in, FILE *out, int *sIn, int *sOut, char *op){
    FILE *start = in;
    char *line = malloc(64 * sizeof(char));
    fgets(line, 64, in);
    if(line[0] >='0' && line[0] <= '9') {
        if(!(sscanf(line, "%i %i", sIn, sOut) == 2)){
        in = start;
        free(line);
        return 0;
        }
        fprintf(out,"%i %i\n", *sIn, *sOut);
        free(line);
        return 1;
    }
    else {
        if (!(*op == '+' || *op == '^' || *op == '%' || *op == '/' || *op == '*')) return 0;
        if(!(sscanf(line, "%c %i", op, sOut))){
        //in = start;
        free(line);
        return 0;
        }
        fprintf(out,"%c %i\n", *op, *sOut);
        free(line);
        return 2;
    }
}

int skipEmpty(FILE *in){
    char* linia = malloc(MAX_L *sizeof(char));
    fgets(linia, 16, in);
    char first = linia[0];
    free(linia);
    if(first != '\n') return 0;
    return 1;
}

int arithmetic(FILE *in, FILE *out, char op, int sOut, char *firstArg){
    char *line = malloc(MAX_L * sizeof(char));
    char *result = malloc(MAX_L * sizeof(char));
    char *arg = malloc(MAX_L * sizeof(char));

    strcpy(result,firstArg);

    fprintf(out, "%s\n", result);
    removeZero(result);

    fgets(line, MAX_L, in); //blank

    while (fgets(line, MAX_L, in) && line[0] != '\n') { 
        
        // kolejny argument
        line[strlen(line) - 1] = '\0';  // Usunięcie końca linii
        strcpy(arg,line); 
        fprintf(out, "%s\n", arg);
        removeZero(arg);
        
        // Wybór operacji
        char* newResult = NULL;
        
        //printf("%s\n", result);
        //printf("%s\n", arg);
        switch (op)
        {
        case '+':
            newResult = addition(result,arg, sOut);
            result = realloc(result, strlen(newResult) + 1);
            strcpy(result,newResult);
            free(newResult);
            break;
        case '%':
            newResult = modulo(result, arg, sOut);
            result = realloc(result, strlen(newResult) + 1);
            strcpy(result,newResult);
            free(newResult);
            break;
        case '/':
            newResult = division(result, arg, sOut);
             
            result = realloc(result, strlen(newResult)+1);
            strcpy(result,newResult);
            free(newResult);
            break;
        case '*':
            newResult = multiplication(result, arg, sOut);
            result = realloc(result, strlen(newResult) + 1);
            strcpy(result,newResult);
            free(newResult);
            break;
        case '^':
            newResult = power(result, arg, sOut);
            result = realloc(result, strlen(newResult) + 1);
            strcpy(result,newResult);
            free(newResult);
            break;
        default:
            fprintf(out,"Not meant to end up here\n");
        }
        //printf("Wynik po operacji %c: %s\n", op, result);  // Informacja debugowania'
        fgets(line, MAX_L, in); //blank
        }
        fprintf(out, "Wynik: %s\n\n", result);
    
}



void processFile(FILE *in, FILE *out){
    int sIn;
    int sOut;
    char operation;
    int ari = -1;
    int argLen = 0;
    
    char *arg1 = malloc(3 * sizeof(char));
    char *arg2 = malloc(3 * sizeof(char));



    char *result;
    char *arg = malloc(MAX_L *sizeof(char));
    char *line = malloc(MAX_L *sizeof(char));

    
    while (fgets(line, MAX_L, in) != NULL){
        argLen = strlen(line);
        if(argLen <= 1) continue;
        
        if(sscanf(line, "%s %s", arg1, arg2) == 2){
            
            if(!(arg1[0] == '+' || arg1[0] == '^' || arg1[0] == '%' || arg1[0] == '/' || arg1[0] == '*')){
                sIn = atoi(arg1);
                ari = 0;
            }
            else{
                operation = arg1[0];
                ari = 1;
            }
            sOut = atoi(arg2);
            if(ari) fprintf(out, "%c %i\n", operation, sOut);
            else fprintf(out, "%i %i\n", sIn, sOut);
            continue;
        }
        
        if(ari == 1){
            strcpy(arg, line);
            argLen = strlen(arg);
            arg[argLen - 1] = '\0';
            removeZero(arg);
            arithmetic(in, out, operation, sOut, arg);
            free(arg);
            continue;
        }
        else{
            strcpy(arg, line);
            argLen = strlen(arg);
            arg[argLen - 1] = '\0';
            removeZero(arg);
            fprintf(out,"%s\n", arg);
            result = systemChange(arg, sIn, sOut);
            fprintf(out,"WYNIK: %s\n\n", result);
            continue;
        }
    }
 
}

