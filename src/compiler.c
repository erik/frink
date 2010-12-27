#include "compiler.h"

static Instruction pushConstant(AttoBlock* b, TValue t) {
  int index = -1;
  int i;
  Vector *v = b->k;
  for(i = 0; i < v->size && index == -1; ++i) {
    TValue tv = getIndex(v, i);

    if(tv.type == t.type) {
      switch(t.type) {
      case TYPE_NUMBER:
	if(TV2NUM(t) == TV2NUM(tv)) {
	  index = i;
	}
	break;
      case TYPE_STRING:
	if(strcmp(TV2STR(tv), TV2STR(t)) == 0) {
	  index = i;
	}
	break;
      case TYPE_NULL:
        break;
      default:
	fprintf(stderr, "Unknown type: %d\n", t.type);
	return -1;
      }
    }
  }
  
  if(index != -1) {
    return index;
  }

  return append(b->k, t) - 1;
}

/* Macros for laziness */
#define IS(x, y) (strcmp(x, y) == 0)
#define PUSH(op) AttoBlock_push_inst(b, op)
#define IF(op)   if(IS(word, op))
#define EIF(op)  else IF(op)
static void compileWord(AttoBlock *b, char* word) {
  IF("pop") {
    PUSH(OP_POP);
  } EIF("dup") {
    PUSH(OP_DUP);  
  } EIF("swap") {
    PUSH(OP_SWAP);
  } EIF("+") {
    PUSH(OP_ADD);
  } EIF("-") {
    PUSH(OP_SUB);
  } EIF("*") {
    PUSH(OP_MUL);
  } EIF("/") {
    PUSH(OP_DIV);
  } EIF("%") {
    PUSH(OP_MOD);
  } EIF("^") {
    PUSH(OP_POW);
  } EIF (".") {
    PUSH(OP_PRINT);
  } else {
    fprintf(stderr, "Unknown word: %s\n", word);
  }
}
#undef PUSH
#undef IS
#undef IF
#undef EIF

// TODO: when more than 3 constants, first opcode's value gets wrecked 
// pushing NOP as first inst seems to fix this, but still needs to be fixed

AttoBlock* compileFrink(FrinkProgram* fp) {
  AttoBlock* b = AttoBlockNew();

  AttoBlock_push_inst(b, OP_NOP);

  int i;
  for(i = 0; i < fp->len; ++i) {
      Token t = fp->tokens[i];

      switch(t.type) {
      case TOKEN_WORD: {
	compileWord(b, t.content);
	break;
      }
      case TOKEN_STRING: {
	TValue tv;
	AttoString str;
	Value v;
	str.len = strlen(t.content);
	str.ptr = t.content;
	v.string = str;
	tv.type = TYPE_STRING;
	tv.value = v;

	Instruction in = pushConstant(b, tv);
	AttoBlock_push_inst(b, (int)OP_PUSHCONST);
	AttoBlock_push_inst(b, in);

	break;
      }
      case TOKEN_NUMBER: {
	AttoNumber d = strtod(t.content, NULL);

	Instruction in = pushConstant(b, createNumber(d));
	AttoBlock_push_inst(b, (int)OP_PUSHCONST);
	AttoBlock_push_inst(b, in);

	break;
      }
		
      case TOKEN_UNKNOWN:
      default:
        if(strcmp(t.content, "NULL")) {
          fprintf(stderr, "Unknown token: `%s'\n", t.content);
        }
    }
  }

  AttoBlock_push_inst(b, OP_NOP);

  return b;

}
