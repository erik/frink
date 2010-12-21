#include <ctype.h>
#include <string.h>

#include "loader.h"
#include "token.h"

static char * currWord;

static long LineLength(FILE* fp) {
  fpos_t start;
  long int bol;
  long int eol;

  fgetpos (fp, &start);
  bol = ftell(fp);
  char c;  
  while((c = fgetc(fp)) != EOF && (c != '\n' || c != '\r')) {}

  eol = ftell(fp);

  fsetpos (fp, &start);

  printf("line len => %ld\n", eol - bol);
  return eol - bol;

}

static char* ReadLine(FILE* fp) {
  long len = LineLength(fp);
  if(len <= 1) {
    return NULL;
  }
  char* line = malloc(len);
  line = fgets(line, len, fp);
  return line;
}


//TODO: get rid of me
static char* ReadWord(FILE* fp) {
  char* word = NULL;
  int   allocated = 0;
  char c;
  while((c = fgetc(fp)) != EOF) {
    if(isspace(c)) {
      return word;
    }
    ++allocated;
    if(word != NULL) {
      char* tmp = malloc(allocated);
      strcpy(tmp, word);      
      free(word);
      word = tmp;
    } else {
      word = malloc(allocated);
    }
    word[allocated - 1] = c;
  }
  return word;  
} 

static void ReadWordFromLine(char* line) {
  if(line == NULL) {
    currWord = NULL;
    return;
  }
  if(currWord == NULL) {
    currWord = strtok (line, " \f\n\r\t\v");
  } else {
    currWord = strtok (NULL, " ");
  }
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

//FIXME: get rid of FILE*
                       //char* line
static Token WordToToken(FILE* fp, char* word) {
  if(word == NULL) {
    return MakeToken("NULL");
  }
  int length = strlen(word);

  if(!length) {
    return MakeToken(word);
  }

  if(length == 1) {
    if(word[0] == '"') {
      return StringToken(ReadString(fp));
    } else if(word[0] == '(') {
      char c;
      while((c = fgetc(fp)) != ')' && c != EOF) {}
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
  
  int numtokens = 0;
  Token* tokens = NULL;

  currWord = NULL;

  while(!feof(fp)) {
    char *line = ReadLine(fp);
    if(line == NULL || feof(fp)) {
      break;
    }
    do {
      ReadWordFromLine(line);
      printf("word => %s\n", currWord);
      Token t = WordToToken(fp, currWord);
      
      Token* tmp = realloc(tokens, sizeof(Token) * ++numtokens);
      tokens = tmp;
      tokens[numtokens - 1] = t;
    } while(currWord != NULL);
  }

  frink->tokens = tokens;
  frink->len = numtokens;
  return frink;
}
