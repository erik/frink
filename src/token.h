#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "frink.h"

typedef enum {
  TOKEN_WORD,
  TOKEN_STRING,
  TOKEN_NUMBER,
  TOKEN_UNKNOWN
} TokenType;

typedef struct {
  char* content;
  int size;
  TokenType type;
} Token;

Token MakeToken(char*);
Token WordToken(char*);
Token StringToken(char*);
Token NumberToken(char*);

#endif /* _TOKEN_H_ */
