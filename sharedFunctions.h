#ifndef SHARED_FUNCTIONS_H
#define SHARED_FUNCTIONS_H

#include "numberArray.h"

void autoGen(NumberArray* arr);
int isInteger(const char* input);
void addTxtExtensionIfMissing(char* fileName);
int fileExists(const char* filename);
int isValidFileName(const char* name);

#endif