#include <ctype.h>
#include <string.h>

#include "loader.h"
#include "token.h"
/*
static char *  currWord;
static char *  currLine;
static size_t  lineIndex;*/

static int     isEOL;
static int     isEOF;


static char* ReadWord(FILE* fp) {
  if(isEOF) {
    return NULL;
  }

  char c;
  while(isspace(c = fgetc(fp)));
  ungetc(c, fp);
  isEOL = 0;

  long int pos = ftell(fp);
  
  int i = 0;
  while((c = fgetc(fp))) {
    ungetc(c, fp);
    fseek(fp, 1, SEEK_CUR);
    if(c == EOF || feof(fp)) {
      isEOL = isEOF = 1;
      break;
    } else if(isspace(c)) {
      if(c == '\n' || c == '\r') isEOL = 1;
      break;
    } else {
      isEOL = isEOF = 0;
      i++;
    }
  }

  fseek(fp, pos, SEEK_SET);

  char *word = malloc(i + 1);
  fread(word, 1, i, fp);
  word[i] = '\0';

  return word;
}

static char* ReadDelim(FILE* fp, char delim) {
  char c;
  int i = 0;

  fgetc(fp);

  long int pos = ftell(fp);

  while((c = fgetc(fp))) {
    ungetc(c, fp);
    fseek(fp, 1, SEEK_CUR);
    if(c == EOF || feof(fp)) {
      isEOL = isEOF = 1;
      break;
    } else if(c == delim) {
      i++;
      break;
    } else {
      if(c == '\n' || c == '\r') {
        isEOL = 1;
      } else {
        isEOL = 0;
      }
      isEOF = 0;
      i++;
    }
  }

  fseek(fp, pos, SEEK_SET);
  
  char *str = malloc(i);
  fread(str, 1, i - 1, fp);
  
  str[i-1] = '\0';

  // get rid of last delim
  fgetc(fp);


  if(isEOF) {
    fprintf(stderr, "Unterminated delim ('%d'): %s\n", delim, str);
    return NULL;
  }

  return str;
}

static char* ReadString(FILE* fp) {
  char* str = ReadDelim(fp, '"');
  return str;
}

static void ReadComment(FILE*fp) {
  char* s = ReadDelim(fp, ')');
  free(s);
  return;
}

static Token WordToToken(FILE* fp, char* word) {
  if(word == NULL) {
    return MakeToken("NULL");
  }
  int length = strlen(word);

  if(!length) {
    return MakeToken(word);
  }

  if(length == 1) {
    /* string */
    if(word[0] == '"') {
      char *str = ReadString(fp);
      if(isEOF || str == NULL) {
        return MakeToken("UNEXPECTED EOF");
      } else {
        return StringToken(str);
      }
      /* comment */
    } else if(word[0] == '(') {
      ReadComment(fp);
      return WordToToken(fp, ReadWord(fp));
    }
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
    return NumberToken(word);
  } 

  return WordToken(word);
}

FrinkProgram* LoadFile(FILE* fp, char* name) {
  FrinkProgram* frink = malloc(sizeof(FrinkProgram));
  frink->source = name;
  
  isEOL = isEOF = 0;

  int numtokens = 0;
  Token* tokens = NULL;



  while(!feof(fp) && !isEOF) {
    char *word = ReadWord(fp);

    if(word == NULL || isEOF) {
      break;
    }

    Token t = WordToToken(fp, word);
    
    printf("Token added: %s\n", t.content);
    
    Token* tmp = realloc(tokens, sizeof(Token) * ++numtokens);
    tokens = tmp;
    tokens[numtokens - 1] = t;
  }

  frink->tokens = tokens;
  frink->len = numtokens;
  return frink;
}
