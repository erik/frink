/*   This file is part of Frink.
 *
 *   Frink is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Frink is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Frink.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "compiler.h"

static Instruction pushConstant(AttoBlock* b, TValue t) {
  int index = -1;
  unsigned i;
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
      case TYPE_BOOL:
        if(tv.value.bool == t.value.bool) {
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
// return number of tokens to skip, or -1 for error. messy
static int compileWord(FrinkProgram* fp, int tokIndex, AttoBlock *b, char* word) {
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
  } EIF("true") {
    PUSH(OP_PUSHCONST);
    PUSH(pushConstant(b, createBool(1)));
  } EIF("false") {
    PUSH(OP_PUSHCONST);
    PUSH(pushConstant(b, createBool(0)));
  } EIF("?") {
    PUSH(OP_BOOLVALUE);
  } EIF (".") {
    PUSH(OP_PRINT);
  } EIF(".cr") {
    PUSH(OP_PRINT);
    PUSH(OP_PUSHCONST);
    PUSH(pushConstant(b, createString("\n", 2, 1)));
    PUSH(OP_PRINT);
  } EIF("cr") {
    PUSH(OP_PUSHCONST);
    PUSH(pushConstant(b, createString("\n", 2, 1)));
    PUSH(OP_PRINT);
  } EIF("gets") { 
    PUSH(OP_READLINE);
  } EIF("concat") {
    PUSH(OP_CONCAT);
  } EIF(".S") {
    PUSH(OP_DUMPSTACK);
  } EIF("clear") {
    PUSH(OP_CLEARSTACK);
  } EIF("var"){
    if(tokIndex < fp->len - 1) {
      Token nextToken = fp->tokens[++tokIndex];
      if(FrinkProgram_add_var(fp, nextToken.content)) {
        fprintf(stderr, "Var already defined: %s\n", nextToken.content);
        return -1;
      }
    } else {
      fprintf(stderr, "Expected variable name, but got EOF\n");
      return -1;
    }
    return 1;
  } EIF("set") {
    PUSH(OP_SETVAR);
  } EIF("value") {
    PUSH(OP_VALUEVAR);
  } EIF("constant") {
    if(tokIndex < fp->len - 2) {
      Token name  = fp->tokens[++tokIndex];
      Token value = fp->tokens[++tokIndex];

      int ind = FrinkProgram_find_const(fp, name.content);
      
      if(ind != -1) {
        TValue tv = Token_to_TValue(fp->kval[ind]);
        fprintf(stderr, "Constant `%s' already defined with value `%s' (%s)\n", name.content, TValue_to_string(tv), TValue_type_to_string(tv));
        return -1;
      }
      
      TValue tv = Token_to_TValue(value);

      int constIndex = pushConstant(b, tv);

      FrinkProgram_add_const(fp, name.content, constIndex, value);
      return 2;
    }
    
    fprintf(stderr, "Expected constant name and value, but got EOF\n");
    return -1;

  } else {
    // check if word is a var
    int index = FrinkProgram_find_var(fp, word);

    if(index != -1) {

      PUSH(OP_PUSHVAR);
      PUSH((Instruction)index);
      return 0;
    }

    index = FrinkProgram_find_const(fp, word);
    
    if(index != -1) {
      
      int constIndex = fp->kind[index];
      
      PUSH(OP_PUSHCONST);
      PUSH((Instruction)constIndex);
      return 0;
    }

    fprintf(stderr, "Unknown word or var: %s\n", word);
    return -1;
  }
  return 0;
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
	int v = compileWord(fp, i, b, t.content);
        if(v == 0) {
        } else if( v != -1) {
          i += v;
        } else {
          fprintf(stderr, "Error during compile. Abort\n");
          return NULL;
        }
	break;
      }
      case TOKEN_STRING: {
        TValue tv = Token_to_TValue(t);
        Instruction in = pushConstant(b, tv);
	AttoBlock_push_inst(b, (int)OP_PUSHCONST);
	AttoBlock_push_inst(b, in);
        
	break;
      }
      case TOKEN_NUMBER: {
        TValue tv = Token_to_TValue(t);
        Instruction in = pushConstant(b, tv);
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

  b->sizev = fp->numvars;

  return b;

}


TValue Token_to_TValue(Token t) {
  TValue tv;
  
  if(t.type == TOKEN_STRING) {
    tv = createString(t.content, strlen(t.content) + 1, 1);
  } else if(t.type == TOKEN_NUMBER) {
    AttoNumber n = strtod(t.content, NULL);
    tv = createNumber(n);
  } else if(t.type == TOKEN_BOOL) {
    AttoNumber n = strtod(t.content, NULL);
    tv = createBool((n ? 1 : 0));
  } else {
    fprintf(stderr, "Unrecognized type: %d\n", t.type);
    tv = createNull();
  }

  return tv;

}
