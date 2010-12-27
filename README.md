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
