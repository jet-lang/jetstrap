# jetstrap
A version of the Jet compiler written in C++. This compiler is bare-bones and only exists for porting the main Jet compiler to new platforms.

## building
Make sure you have a C++ toolchain available on your system, such as Clang or GCC.

- `git clone https://github.com/jet-lang/jetstrap`
- `make && make install`

Once you jetstrap the main Jet compiler, it is recommended that you recompile the main Jet compiler again using the jetstrapped Jet compiler. This way, you can recompile the main Jet compiler with all of the proper optimizations.
