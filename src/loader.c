#include <ctype.h>
#include <string.h>

#include "loader.h"
#include "token.h"

static char *  currWord;
static char *  currLine;
static size_t  lineIndex;

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

  return eol - bol;
}

static char* ReadLine(FILE* fp) {
  lineIndex = 0;
  long len = LineLength(fp);
  if(len <= 1) {
    return NULL;
  }
  char* line = malloc(len + 1);
  line = fgets(line, len, fp);

  size_t i;
  for(i = 0; i < strlen(line); ++i) {
    if(line[i] == '\n') {
      line[i] = '\0';
      break;
    }
  }

  printf(">>%s<<\n", line);
  return line;
}

static void ReadWordFromLine() {
  if(currLine == NULL) {
    currWord = NULL;
    return;
  }
  if(currWord == NULL) {
    currWord = strtok (currLine, " \f\n\r\t\v");
  } else {
    currWord = strtok (NULL, " ");
  }

  if(currWord) {   
    while(currLine[lineIndex++] != 0);
  } else {
    lineIndex = -1;
  }
}

static char* ReadString(char* line) {
  int pos = lineIndex;
  char* str = malloc(strlen(line) - pos);
  size_t ctr;  

  for(ctr = pos; ctr < strlen(line); ++ctr) {
    char c = line[ctr];
    if(c == EOF || c == 0) {
      break;
    } else if (c == '"') {
      return str;
    } else {
      str[ctr] = c;
    }
  }

  /* if here, string isn't terminated on line */
  fprintf(stderr, "Unterminated string: %s\n", str);
  return NULL;
}

static Token WordToToken(char* line, char* word) {
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

      return StringToken(ReadString(line));

      /* comment */
    } else if(word[0] == '(') {

      while(currWord) {
        ReadWordFromLine();
        if(currWord && strlen(currWord) == 1 && currWord[0] == ')') {
          ReadWordFromLine();
          break;
        }
      }

      if(lineIndex >= strlen(line)) {
        fprintf(stderr, "Unterminated comment on line: %s\n", line);
        return WordToToken(NULL, NULL);
      }

      return WordToToken(line, currWord);
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
    if(!line) {
      break;
    }
    char *fakeLine = malloc(strlen(line));
    strcpy(fakeLine, line);
    currLine = fakeLine;

    if(line == NULL || feof(fp)) {
      break;
    }
    do {
      ReadWordFromLine();
      Token t = WordToToken(line, currWord);
      
      printf("Token added: %s\n", t.content);

      Token* tmp = realloc(tokens, sizeof(Token) * ++numtokens);
      tokens = tmp;
      tokens[numtokens - 1] = t;
    } while(currWord != NULL);
  }

  frink->tokens = tokens;
  frink->len = numtokens;
  return frink;
}
