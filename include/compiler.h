#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "block.h"
#include "loader.h"

AttoBlock* compileFrink(FrinkProgram*);
TValue Token_to_TValue(Token);
#endif /* _COMPILER_H_ */

