#ifndef READ_H
#define READ_H

#include <stdio.h>

void processFile(FILE *in, FILE *out);

char *createOutName(char *in);

#endif /*READ_H*/