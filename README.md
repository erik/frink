#frink

##About
Frink is an implementation of the Forth programming language, with the
Atto virtual machine as its compile target. Frink reads .fk, such as
those in `test/` and compiles them down to Atto bytecode.

I plan to allow the compile/run process to be done directly from
Frink, without having to run Atto, or generate bytecode. This will be
added eventually, but right now, it can only compile to bytecode.

##Usage
Frink depends on Atto to build. After cloning this repository, running
`make` will clone the Atto repository and build it
automatically. Running `make distclean all` will remove the old Atto
lib, and download and build fresh version of the repository.

After building frink, compile frink files with `./frink some/file.fk`.
This will generate `out.ato`, which can be run by the Atto virtual
machine executable.

##Language
Frink is an implementation of Forth, and as such is a stack
based language. There are a bunch of standard Forth-ish words, like
`dup` `swap`, `pop`, .... 
###Constants
In Frink you can define constants, which are immutable values,
probably used frequently in code. They basically act like #define does
in C. They are set with `constant <name> <value>` and can be used
throughout the code instead of using the actual value. Value can be
either a string or a number.
Here's an example of how to use them:
       constant number 42
       number .cr
###Vars
Vars are sort-of kind-of like pointers in C (sort of). They cannot be
accessed directly, but must be set and read with the special words
`set` and `value`. A var is declared by `var <name>`
Here's an example:
       var x
       1 x  set ( sets the var)
       x .cr    ( doesn't print 1!)
                ( prints something like `Var@0xbf8cdb20')
       x value .cr ( prints 1)
##License
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
