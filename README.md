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

Copyright (c) 2010, 2011 Erik Price

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
