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

