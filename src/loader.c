#include <ctype.h>

#include "loader.h"
#include "token.h"

static char* ReadWord(FILE* fp) {
  char* word = NULL;
  int   allocated = 0;
  char c;
  while((c = fgetc(fp)) != EOF) {
    if(isspace(c)) {
      return word;
    }
    char* tmp = malloc(++allocated);
    strcpy(tmp, word);      
    free(word);
    word = tmp;
    word[allocated - 1] = c;
  }
  return word;  
}

static char* ReadString(FILE* fp) {
  char* str = NULL;
  int   allocated = 0;
  
  char c;
  while((c = fgetc(fp))) {
    if(c == EOF) {
      fprintf(stderr, "Unterminated string: %s\n", str);
      return NULL;
    } else if (c == '"') {
      return str;
    } else {
      char* tmp = malloc(++allocated);
      strcpy(tmp, str);
      free(str);
      str = tmp;
      str[allocated - 1] = c;
    }
  }
  return str;
}

static Token WordToToken(FILE* fp, char* word) {
  int length = strlen(word);

  if(!length) {
    return MakeToken(word);
  }

  if(length == 1 && word[0] == '"') {
    return StringToken(ReadString(fp));
  }

  if(isdigit(word[0])) {
    int dot_seen = 0;
    int i;
    for(i = 1; i < length; ++i) {
      char c = word[i];
      if(isdigit(c)) {
	continue;
      } else if (c == '.') {

	if(dot_seen) {
	  return WordToken(word);
	} else {
	  dot_seen++;
	}

      } else {
	return WordToken(word);
      }
    }
  } 

  return WordToken(word);
}

FrinkProgram* LoadFile(FILE* fp, char* name) {
  FrinkProgram* frink = malloc(sizeof(FrinkProgram));
  frink->source = name;
  
  int numtokens = 0;
  Token* tokens = NULL;

  while(!feof(fp)) {
    char *word = ReadWord(fp);
    Token t = WordToToken(fp, word);
    
    Token* tmp = malloc(++numtokens);
    int i;
    for(i = 0; i < numtokens - 1; ++i) {
      tmp[i] = tokens[i];
    }

    tmp[i] = t;
    
    free(tokens);
    tokens = tmp;
  }
  frink->tokens = tokens;
  frink->len = numtokens;
  return frink;
}
