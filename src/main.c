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

#include <stdio.h>
#include <stdlib.h>

#include "frink.h"
#include "loader.h"
#include "compiler.h"
#include "block.h"
#include "vm.h"
#include "dump.h"

static int writer(AttoVM* vm, const void *p, size_t s, void *d) {
  UNUSED(vm);
  return (fwrite(p, s, 1, (FILE*)d) != 1) && (s != 0);
}

static int usage() {
  fprintf(stderr, "Usage:\n"
	  "frink [options] file\n");
  return 1;
}

int main(int argc, char** argv) {

  if(argc < 2) {
    return usage();
  }


  FILE* fp = fopen(argv[1], "r");
  char* name = argv[1];

  FrinkProgram *f = LoadFile(fp, name);
  AttoBlock* b = compileFrink(f);

  fclose(fp);

  AttoVM* vm = AttoVMNew();

  Proto* p = Proto_from_block(vm, b);
  fp = fopen("out.ato", "wb");
  dump(vm, p, writer, fp);

  fclose(fp);
  AttoVMDestroy(vm);
  AttoBlockDestroy(b);

  return 0;

}

