#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "calc.h"

#define MAX_L 1024


int checkBase(char *a, int base){ 
    char z;
    int i;
    for(i = 0; i < strlen(a); i++){
        z = a[i];
        if(!((z >= '0' && z <= '9') || (z >= 'A' && z <= 'F'))) return 0;
        if(getValue(z) >= base) return 0;
    }
    return 1;
}

int checkSystem(char *arg){
    int l;
    if(!isdigit(arg[0])) return 0;
    l = atoi(arg);
    return l >= 1 && l <= 16;
}

int checkOp (char *arg){
    if (strlen(arg) == 1 && strchr("+*/%^", arg[0])) return 1;
    return checkSystem(arg);
}

int checkHeader(char *arg1, char *arg2){
    return checkOp(arg1) && checkSystem(arg2);
}


void processFile(FILE *in, FILE *out){
    int sIn;
    int sOut;
    char operation;

    int opFlag = -1;
    int argLen = 0;
    int emptyCounter = 0;

    char *arg1 = malloc(3 * sizeof(char));
    char *arg2 = malloc(3 * sizeof(char));

    char *line = malloc(MAX_L *sizeof(char));
    char *argument = malloc(MAX_L *sizeof(char));
    char *result, *newResult;
    result = NULL;
    newResult = NULL;

    while (fgets(line, MAX_L, in) != NULL){
        argLen = strlen(line);
        if(argLen <= 1){
            emptyCounter += 1;
            if(emptyCounter == 3 && result != NULL){
                fprintf(out, "\nWynik = %s\n\n", result);
                result = NULL;
            }
            continue;
        }
        emptyCounter = 0;

        if(sscanf(line, "%s %s", arg1, arg2) == 2){
            if(result != NULL){
                fprintf(out, "\nWynik = %s\n\n", result);
                free(result);
                result = NULL;
            }
            if(!checkHeader(arg1, arg2)){
                fprintf(out, "Zly format naglowka\n\n");
                opFlag = -1;
                continue;
            }
            if(!(arg1[0] == '+' || arg1[0] == '^' || arg1[0] == '%' || arg1[0] == '/' || arg1[0] == '*')){
                sIn = atoi(arg1);
                opFlag = 2;
            }
            else{
                operation = arg1[0];
                opFlag = 1;
            }
            sOut = atoi(arg2);
            if(opFlag == 1) fprintf(out, "%c %i\n\n", operation, sOut);
            else fprintf(out, "%i %i\n\n", sIn, sOut);
            
            continue;
        }
        strcpy(argument, line);
        argument[argLen - 1] = '\0';
        if(opFlag == -1){
            fprintf(out, "Dla liczby nie zdefiniowano dzialania\n");
            continue;
        }
        
        if(opFlag == 1){
            if(!checkBase(argument,sOut)){
                fprintf(out,"'%s' nie jest liczba w systemie %i\n", argument, sOut);
                continue;
            }
            removeZero(argument);
            fprintf(out, "%s\n", argument);
            if(result == NULL){
                result = malloc((strlen(argument) + 1) * sizeof(char));
                strcpy(result,argument);
                continue;
            }
            
            switch (operation)
            {
                case '+':
                    newResult = addition(result, argument, sOut);
                    break;

                case '%':
                    newResult = modulo(result, argument, sOut);
                    break;

                case '/':
                    newResult = division(result, argument, sOut);
                    break;
                case '*':
                    newResult = multiplication(result, argument, sOut);
                    break;

                case '^':
                    newResult = power(result, argument, sOut);
                    break;

                default:
                    fprintf(out,"Not meant to end up here\n");
                }
            
            free(result);
            result = newResult;
        }
        
        
        if(opFlag == 2){
            if(checkBase(argument, sIn)) fprintf(out, "%s\n", argument);
            else{
                fprintf(out, "Liczba '%s' nie jest zapisana w systemie %i\n\n",argument, sIn);
                continue;
            }
            removeZero(argument);
            result = systemChange(argument, sIn, sOut);
            fprintf(out,"\nWynik: %s\n\n", result);
            free(result);
            result = NULL;
        }
    }
}
char *createOutName(char *in){
    char *result = malloc(strlen(in) + 6);
    strcpy(result, "out_");
    strcat(result, in);
    return result;
}
