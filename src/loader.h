#ifndef _LOADER_H_
#define _LOADER_H_

#include "frink.h"
#include "token.h"

typedef struct {
  char* source;
  Token* tokens;
  int len;
} FrinkProgram;


FrinkProgram* LoadFile(FILE*, char*);
void DestroyFrinkProgram(FrinkProgram*);

#endif /* _LOADER_H_ */
