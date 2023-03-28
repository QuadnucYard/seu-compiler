# seu-compiler

Repository for _Compilation Principles Practice_ in Southeast University.

## Descriptions

SEU-Lex + SEU-Yacc + ...

## Requirements

To build and run this project, you need some environment configurations.

1. [XMake](https://github.com/xmake-io/xmake), currently v2.7.6. To build this project. Recommend: XMake in VS Code.
2. [MinGW64 (Least 12.0)](https://github.com/niXman/mingw-builds-binaries/releases), or MSVC (At least 19.3) installed with [Visual Studio 2022](https://visualstudio.microsoft.com/). The C++ compiler is required to support fundamental [C++23 features](https://en.cppreference.com/w/cpp/23).

> Some packages (e.g., `tl-ranges`) are not included in `xrepo`. Thus we use git submodule as the package source, instead of `xrepo`. But you can also modify `xmake.lua` to adapt to your own package manager.

### Note

We notice that MSVC has better intellisense in intensive template usage scenerios, but it has poor, confusing compiling information over G++.

According to incomplete compilation tests, the code can be built either with mingw(G++) or windows(MSCV). For compatibility, we avoid some features like `__int128` in gcc. If you find compilation problems, please check the version of compiler first, as the project is developed with the latest toolchain.
