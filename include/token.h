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

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "frink.h"

typedef enum {
  TOKEN_WORD,
  TOKEN_STRING,
  TOKEN_NUMBER,
  TOKEN_BOOL,
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

