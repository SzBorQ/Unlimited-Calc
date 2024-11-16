#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"


int getValue(char x){
    if((x >= '0') && (x <= '9')) return x - '0';
    if((x >= 'A') && (x <= 'F')) return x - 'A' + 10;
    
}

char saveValue(int x){
    if(x < 10) return x + '0';
    if((x >= 10) && (x <= 16)) return ('A' + x - 10);   
}

char *saveDec(int x){
    char *result = malloc(3 * sizeof(char));
    if (x < 10){
        result[0] = '0' + x;
        result[1] = '\0';
    }
    else{
        result[0] = '1';
        result[1] = '0' + (x%10);
        result[2] = '\0';
    } 
    return result;
}


void removeZero(char *a){
    while(a[0] == '0' && a[1] != '\0'){
        memmove(a,a+1,strlen(a));
    }
}

char *addition(char *a, char *b, int base){
    
    //ostatnia cyfra dodawanych liczb
    int lenA = strlen(a) - 1;
    int lenB = strlen(b) - 1;

    int curV, vA, vB; // tymczasowo na sumę
    int carry = 0;
    int i = 0;
    char *wynik = malloc((lenA > lenB ? (lenA + 2) : (lenB + 2)) * sizeof(char));

    while( (lenA >= 0) || (lenB >= 0) || carry){

        vA = (lenA >= 0) ? getValue(a[lenA]) : 0; 
        vB = (lenB >= 0) ? getValue(b[lenB]) : 0;
        curV = vA + vB + carry;
        carry = curV / base;
        curV = curV % base;
        wynik[i] = saveValue(curV);

        
        lenA--;
        lenB--;
        i++;
        
    }
    wynik[i] = '\0';

    for (int j = 0; j < (i / 2); j++) {
        char temp = wynik[j];
        wynik[j] = wynik[i - 1 - j];
        wynik[i - 1 - j] = temp;
    }

    return wynik;
       
}

int comparsion(char *a, char *b){
    if (strlen(a) != strlen(b)) return strlen(a) - strlen(b);
    return strcmp(a,b);
}

char *subtraction(char *a, char *b, int base){
    int lenA = strlen(a);
    int lenB = strlen(b);

    int carry = 0;
    int curV;
    int i = lenA - 1;
    int j = lenB - 1;
    int k = lenA;
    
    char *wynik = malloc((lenA > lenB ? (lenA + 2) : (lenB + 2)) * sizeof(char));
    wynik[k] = '\0';
    while(i >= 0){
        int dA = getValue(a[i]);
        int dB = (j >= 0) ? getValue(b[j]) : 0;
        
        //printf("%i %i\n", dA, dB);
        curV = dA - dB - carry;
        //printf("%i\n", curV);
        if(curV < 0){
            curV += base;
            carry = 1;
        }
        else{
            carry = 0;
        }

        wynik[--k] = saveValue(curV);
        //printf("%c\n", wynik[k]);
        i--;
        j--;
    }
    //printf("%s\n", wynik);
    removeZero(wynik);
    return wynik;
}

char *multByDigit(char *a, int dig, int base){
    int lenA = strlen(a) - 1;
    char *wynik = malloc((lenA + 1) * sizeof(char));

    int carry = 0;
    int curV = 0;
    int k = 0;
    int i = lenA;
    int curD;
    while((i >= 0) || carry){
        curD = (i >= 0) ? getValue(a[i]) : 0;
        curV = dig * curD + carry;
        wynik[k] = saveValue(curV % base);
        carry = curV / base;
        i--;
        k++;
    }
    wynik[k] = '\0';
    
    for (int j = 0; j < (k / 2); j++) {
        char temp = wynik[j];
        wynik[j] = wynik[k - 1 - j];
        wynik[k - 1 - j] = temp;
    }
    
    return wynik;
    
}

char *multiplication(char *a, char *b, int base){
    int lenA = strlen(a) - 1;
    int lenB = strlen(b) - 1;
    int lenW = lenA + lenB + 2;
    int *intWynik = calloc(lenW, sizeof(int));

    int curV = 0;
    int dA, dB;

    for (int i = lenA; i >= 0; i--){
        dA = getValue(a[i]);
        for(int j = lenB; j >= 0; j--){
            dB = getValue(b[j]);
            curV = dA * dB;

            int pos = i + j + 1;
            intWynik[pos] += curV;
            
            while (intWynik[pos] >= base) {
                intWynik[pos - 1] += intWynik[pos] / base;
                intWynik[pos] %= base;
                pos--;
            }
        }
    }

    char *wynik = malloc((lenA + lenB + 3) * sizeof(char));
    int index = 0;
    int start = 0;

    while(start < lenW && intWynik[start] == 0) start++;

    for(int i = start; i < lenW; i++) wynik[index++] = saveValue(intWynik[i]);
    wynik[index] = '\0';
    free(intWynik);
 
    return wynik;
}

char *str_duplicate(char *a){
    char *dupli = malloc(strlen(a)+1);
    if (dupli) strcpy(dupli, a);
    return dupli;
}

char *division(char* a, char *b, int base){
    if(comparsion(a,b) < 0){
        char *result = malloc(2 * sizeof(char));
        result[0] = '0';
        result[1] = '\0';
        return result;
    }
    int lenA = strlen(a);
    char *result = malloc((lenA + 1) * sizeof(char));
    char *remainder = malloc((lenA + 1) * sizeof(char));
    
    remainder[0] = '\0';
    int resultIndex = 0;
    int q;
    for(int i = 0; i < lenA; i++){
        int len = strlen(remainder);
        remainder[len] = a[i];
        remainder[len + 1] = '\0';
        //printf("check\n");
        q = 0;
        removeZero(remainder);
        //printf("r %s\n", remainder);
        while(comparsion(remainder,b) >= 0){
            char *temp = subtraction(remainder,b,base);
            strcpy(remainder,temp);
            free(temp);
            q++;
        }
        result[resultIndex++] = saveValue(q);
        
    }
    result[resultIndex] = '\0';

    removeZero(result);
    return result;
}

char *modulo(char *a, char *b, int base){
    return subtraction(a,multiplication(b,division(a,b,base),base),base); 
}


char *systemChange(char *a, int bIn, int bOut) {
    char *base10 = malloc(2 * sizeof(char));
    strcpy(base10, "0");

    char *baseIn = saveDec(bIn);
    char *baseOut = saveDec(bOut);


    //printf("%s %s\n", baseIn, baseOut);

    char *multiplier = malloc(2 * sizeof(char));
    strcpy(multiplier, "1");

    char *digit = malloc(3 * sizeof(char));
    int digitValue;

    int lenA = strlen(a);
    for(int i = lenA - 1; i >= 0; i--){
        digitValue = getValue(a[i]);
        digit = saveDec(digitValue);

        //printf("%s\n", digit);
        char *temp = multiplication(multiplier, digit, 10);
        char *newBase10 = addition(base10, temp, 10);

        //printf("Cyfra z wagą %s\n", temp);
        //printf("Nowa dec %s\n", newBase10);

        free(base10);
        base10 = newBase10;

        char *newMultiplier = multiplication(multiplier, baseIn, 10);
        //printf("nowa waga %s\n", newMultiplier);
        free(multiplier);
        multiplier = newMultiplier;
    }

    //printf("%s\n", base10);
    if (bOut == 10){
        free(baseIn);
        free(baseOut);
        free(multiplier);
        return base10;
    } 

    char *result = malloc((4 * lenA+ 1) * sizeof(char));

    int resultIndex = 0;
    char *zero = malloc(2 * sizeof(char));
    strcpy(zero, "0");
    //printf("%s\n", baseOut);
    while(comparsion(base10, zero) > 0){
        char *remainder = modulo(base10, baseOut, 10);
        //printf("r%s", remainder);
        result[resultIndex] = saveValue(atoi(remainder));
        resultIndex++;
        char *newBase10 = division(base10,baseOut,10);
        //printf("%s\n", base10);
        free(base10);
        base10 = newBase10;
        free(remainder);
    }

    result[resultIndex] = '\0';
    char temp;

    for(int i = 0; i < resultIndex / 2; i++){
        temp = result[i];
        result[i] = result[resultIndex - 1 - i];
        result[resultIndex - 1 - i] = temp; 
    }

    free(base10);
    free(baseIn);
    free(baseOut);
    free(multiplier);
    free(zero);
    return result;

}

char *power(char *base, char *exponent, int systemBase) {
    
    if (strcmp(exponent, "0") == 0) {
        char *result = malloc(2 * sizeof(char));
        strcpy(result, "1");
        return result;
    }

   
    if (strcmp(exponent, "1") == 0) {
        return str_duplicate(base);
    }

    char *result = malloc(2 * sizeof(char));
    strcpy(result, "1");

    char *currentBase = str_duplicate(base);
    char *binExponent = systemChange(exponent, systemBase, 2);

    char *zero = malloc(2 * sizeof(char));
    strcpy(zero, "0");

    char *two = malloc(2 * sizeof(char));
    strcpy(two, "2");

    int lenExp = strlen(binExponent) - 1;
    for(int i = lenExp; i >= 0; i--){
        if(binExponent[i] == '1'){
            char *newResult = multiplication(result, currentBase, systemBase);
            free(result);
            result = newResult;
            }
        char *newBase = multiplication(currentBase, currentBase, systemBase);
        free(currentBase);
        currentBase = newBase;
        
    }
    free(binExponent);
    return result;
}