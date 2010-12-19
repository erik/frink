#include "token.h"

Token MakeToken(char*c) {
  Token t;
  t.content = c;
  t.size = strlen(c);
  t.type = TOKEN_UNKNOWN;
  return t;
}

Token WordToken(char *c) {
  Token t = MakeToken(c);
  t.type = TOKEN_WORD;
  return t;
}

Token StringToken(char* s) {
  Token t = MakeToken(s);
  t.type = TOKEN_STRING;
  return t;
}

Token NumberToken(char* s) {
  Token t = MakeToken(s);
  t.type = TOKEN_NUMBER;
  return t;
}
