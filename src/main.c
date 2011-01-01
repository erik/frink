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
	  "frink [options] file\n"
          "-v\tDisplay version and exit\n"
          "-h\tDisplay this help text\n"
          "-o file\tWrite Atto bytecode to file\n"
          "-c\tDon't compile or run, just check syntax\n");
  return 1;
}

int main(int argc, char** argv) {

  if(argc < 2) {
    return usage();
  }

  int check     = 0;
  int out       = 0;
  char* outfile = NULL;
  char* file    = NULL;

  int i;
  for(i = 1; i < argc; ++i) {
    char *arg = argv[i];
    if(arg[0] == '-') {
      switch(arg[1]) {
      case 'v':
	printf("frink v%d.%d, running on atto v%d.%d\n", FRINK_VERSION_MAJOR, 
               FRINK_VERSION_MINOR, VERSION_MAJOR, VERSION_MINOR);
	return 0;
      case 'h':
	usage();
	return 0;
      case 'o':
        out = 1;
        if(i < argc - 1) {
          outfile = argv[++i];
        } else { 
          fprintf(stderr, "No output file given\n");
          return usage();
        }
        break;
      case 'c':
        check = 1;
        break;
      default:
	fprintf(stderr, "Unrecognized switch -%c\n", arg[1]);
	return usage();
      }
    } else {
      if(file != NULL) {
	return usage();
      }
      file = arg;
    }
  }


  FILE* fp = fopen(file, "r");
  char* name = file;

  if(fp == NULL) {
    fprintf(stderr, "No such file: %s\n", file);
    return EXIT_FAILURE;
  }

  FrinkProgram *f = LoadFile(fp, name);
  AttoBlock* b = compileFrink(f);
  Stack* argStack = StackNew();

  fclose(fp);

  if(b == NULL) {
    fprintf(stderr, "Error occurred during compile. Abort\n");
    return EXIT_FAILURE;
  }

  if(check) {
    puts("Syntax OK.");
    return EXIT_SUCCESS;
  }

  AttoVM* vm = AttoVMNew();

  int status;

  if(out) {
    Proto* p = Proto_from_block(vm, b);
    fp = fopen(outfile, "wb");
    dump(vm, p, writer, fp);
    fclose(fp);
    status = EXIT_SUCCESS;
  } else {
    /* this could probably be extracted to its own function, since
     * it is necessary for vm_interpret to run correctly
     */
    if(b->sizev > 0 ) {
      TValue *null = malloc(sizeof(TValue));
      *null = createNull();
      b->vars = malloc(sizeof(TValue) * b->sizev);
      
      int i;
      for( i = 0; i < b->sizev; ++i) {
        b->vars[i] = createVar(null);
      }
    } else {
      b->vars = NULL;
    }
    
    TValue ret = vm_interpret(vm, b, 0, 0, argStack);
    if(ret.type == TYPE_ERROR) {
      status = EXIT_FAILURE;
    } else {
      status = EXIT_SUCCESS;
    }
    StackDestroy(argStack);
  }

  AttoVMDestroy(vm);
  AttoBlockDestroy(b);

  return status;

}

