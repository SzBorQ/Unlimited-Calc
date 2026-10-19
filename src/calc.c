#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"


int getValue(char x){
    if((x >= '0') && (x <= '9')) return x - '0';
    if((x >= 'A') && (x <= 'F')) return x - 'A' + 10;
    return -1;
}

char saveValue(int x){
    if(x < 10) return (char)(x + '0');
    if((x >= 10) && (x <= 15)) return (char)('A' + x - 10);
    return '?';
}

char *saveDec(int x){
    char *result = malloc(3 * sizeof(char));
    if(result == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
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


void removeZero(char *a){ /*usuwanie wiodacych zer*/
    while(a[0] == '0' && a[1] != '\0'){
        memmove(a,a+1,strlen(a));
    }
}

char *addition(char *a, char *b, int base){
    
    /*ostatnia cyfra dodawanych liczb*/
    int lenA = strlen(a) - 1;
    int lenB = strlen(b) - 1;

    int curV, vA, vB; /*tymczasowo na sumę*/
    int carry = 0;
    int i = 0, j = 0;
    char temp;
    char *wynik = malloc((size_t)((lenA > lenB ? lenA : lenB) + 2));
    if(wynik == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
   

    while( (lenA >= 0) || (lenB >= 0) || carry){ /*dodawanie algorytmem dodawania pisemnego*/

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

    

    for (j = 0; j < (i / 2); j++) { /*"odwrócenie" wyniku*/
        temp = wynik[j];
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
    int dA, dB;
    
    char *wynik = malloc((lenA > lenB ? (lenA + 2) : (lenB + 2)) * sizeof(char));
    if(wynik == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    wynik[k] = '\0';
    while(i >= 0){
        dA = getValue(a[i]);
        dB = (j >= 0) ? getValue(b[j]) : 0;
        
        curV = dA - dB - carry;
 
        if(curV < 0){
            curV += base;
            carry = 1;
        }
        else{
            carry = 0;
        }

        wynik[--k] = saveValue(curV);
  
        i--;
        j--;
    }

    removeZero(wynik);
    return wynik;
}


char *multiplication(char *a, char *b, int base){
    int lenA = strlen(a) - 1;
    int lenB = strlen(b) - 1;
    int lenW = lenA + lenB + 2;
    int *intWynik = calloc(lenW, sizeof(int));
    if(intWynik == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }

    int curV = 0;
    int dA, dB;
    int i, j, pos;

    for (i = lenA; i >= 0; i--){ /*mnożenie algorytmem pisemnym*/
        dA = getValue(a[i]);
        for(j = lenB; j >= 0; j--){
            dB = getValue(b[j]);
            curV = dA * dB;

            pos = i + j + 1;
            intWynik[pos] += curV;
            
            while (intWynik[pos] >= base) {
                intWynik[pos - 1] += intWynik[pos] / base;
                intWynik[pos] %= base;
                pos--;
            }
        }
    }

    char *wynik = malloc((lenA + lenB + 3) * sizeof(char));
    if(wynik == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    int start = 0;

    while(start < lenW && intWynik[start] == 0) start++;

    for(i = start; i < lenW; i++) wynik[index++] = saveValue(intWynik[i]); /*zmiana na tablice znakow*/
    wynik[index] = '\0';
    free(intWynik);
 
    return wynik;
}

char *str_duplicate(char *a){ 
    char *dupli = malloc(strlen(a)+1);
    if(dupli == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    if (dupli) strcpy(dupli, a);
    return dupli;
}

char *division(char* a, char *b, int base){
    if(comparsion(a,b) < 0){ /*jeśli b > a to wynik 0*/
        char *result = malloc(2 * sizeof(char));
        if(result == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
        result[0] = '0';
        result[1] = '\0';
        return result;
    }
    int lenA = strlen(a);
    char *result = malloc((lenA + 1) * sizeof(char));
    if(result == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    char *remainder = malloc((lenA + 1) * sizeof(char));
    if(remainder == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    
    remainder[0] = '\0';
    int resultIndex = 0;
    int q, i, len;
    char *temp;

    for(i = 0; i < lenA; i++){ /*dzielenie algorytmem dzielenia pisemnego*/
        len = strlen(remainder);
        remainder[len] = a[i];
        remainder[len + 1] = '\0';
  
        q = 0;
        removeZero(remainder);
 
        while(comparsion(remainder,b) >= 0){
            temp = subtraction(remainder,b,base);
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
    char *result = malloc(3 * sizeof(char));
    if(b[0] == '0') {
        result = str_duplicate("1");
        return result;
    }
    return subtraction(a,multiplication(b,division(a,b,base),base),base); 
}


char *systemChange(char *a, int bIn, int bOut) {
    char *base10 = malloc(2 * sizeof(char));
    if(base10 == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    strcpy(base10, "0");

    char *baseIn = saveDec(bIn);
    char *baseOut = saveDec(bOut);


    char *multiplier = malloc(2 * sizeof(char));
    if(multiplier == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    strcpy(multiplier, "1");

    char *digit = malloc(3 * sizeof(char));
    if(digit == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    int digitValue, i;

    char *newBase10;
    char *temp;
    char *newMultiplier;

    int lenA = strlen(a);
    for(i = lenA - 1; i >= 0; i--){ /*obliczanie wartosci dziesietnej liczby*/
        digitValue = getValue(a[i]);
        digit = saveDec(digitValue);

    
        temp = multiplication(multiplier, digit, 10);
        newBase10 = addition(base10, temp, 10);


        free(base10);
        base10 = newBase10;

        newMultiplier = multiplication(multiplier, baseIn, 10);

        free(multiplier);
        multiplier = newMultiplier;
    }

    
    if (bOut == 10){ /*jesli do dziesietnej to koniec*/
        free(baseIn);
        free(baseOut);
        free(multiplier);
        return base10;
    } 

    char *result = malloc((4 * lenA+ 1) * sizeof(char));
    if(result == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }

    int resultIndex = 0;
    char *remainder;
    
    char *zero = malloc(2 * sizeof(char));
    if(zero == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    strcpy(zero, "0");
  
    while(comparsion(base10, zero) > 0){ /*zmiana liczby z dziesietnej na system zadany*/
        remainder = modulo(base10, baseOut, 10);
       
        result[resultIndex] = saveValue(atoi(remainder));
        resultIndex++;
        newBase10 = division(base10,baseOut,10);
   
        free(base10);
        base10 = newBase10;
        free(remainder);
    }

    result[resultIndex] = '\0';
    char temporary;

    for(i = 0; i < resultIndex / 2; i++){ /*odwrocenie wyniku*/
        temporary = result[i];
        result[i] = result[resultIndex - 1 - i];
        result[resultIndex - 1 - i] = temporary; 
    }

    free(base10);
    free(baseIn);
    free(baseOut);
    free(multiplier);
    free(zero);
    return result;

}

char *power(char *base, char *exponent, int systemBase) {
    
    char *result = malloc(2 * sizeof(char));
    if(result == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    

    if (strcmp(exponent, "0") == 0) {
        strcpy(result, "1");
        return result;
    }

   
    if (strcmp(exponent, "1") == 0) {
        return str_duplicate(base);
    }

    
    
    strcpy(result, "1");

    char *currentBase = str_duplicate(base);
    char *binExponent = systemChange(exponent, systemBase, 2); /*wykladnik na dwojkowy*/

    char *zero = malloc(2 * sizeof(char));
    if(zero == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    strcpy(zero, "0");

    char *two = malloc(2 * sizeof(char));
    if(two == NULL){
        fprintf(stderr, "Blad alokacji pamieci");
        exit(EXIT_FAILURE);
    }
    strcpy(two, "2");

    int lenExp = strlen(binExponent) - 1;
    int i;

    for(i = lenExp; i >= 0; i--){ /*algorytm szybkiego potegowania*/
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