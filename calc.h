#ifndef CALC_H
#define CALC_H

int getValue(char x);

void removeZero(char *a);

char *addition(char *a, char *b, int base);

char *subtraction(char *a, char *b, int base);

char *multiplication(char *a, char *b, int base);

char *division(char* a, char *b, int base);

char *modulo(char *a, char *b, int base);

char *systemChange(char *a, int bIn, int bOut);

char *power(char *base, char *exponent, int systemBase);

#endif /*CALC_H*/