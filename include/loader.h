#ifndef _LOADER_H_
#define _LOADER_H_

#include "frink.h"
#include "token.h"

typedef struct {
  char* source;
  Token* tokens;
  int len;

  char** vars;
  int numvars;

  char** k;
  Token* kval;
  int  * kind;
  int    numk;

} FrinkProgram;


FrinkProgram* LoadFile(FILE*, char*);
void FrinkProgram_destroy(FrinkProgram*);

// 0 on success, nonzero on var already defined
int FrinkProgram_add_var(FrinkProgram*, char*);
// return index, or -1
int FrinkProgram_find_var(FrinkProgram*, char*);

// 0 on success, nonzero on const already defined
int FrinkProgram_add_const(FrinkProgram*, char*, int, Token);
// return index, or -1
int FrinkProgram_find_const(FrinkProgram*, char*);
#endif /* _LOADER_H_ */

