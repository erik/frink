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

} FrinkProgram;


FrinkProgram* LoadFile(FILE*, char*);
void DestroyFrinkProgram(FrinkProgram*);

// 0 on success, nonzero on var already defined
int FrinkProgram_add_var(FrinkProgram*, char*);
// return index, or -1
int  FrinkProgram_find_var(FrinkProgram*, char*);

#endif /* _LOADER_H_ */

