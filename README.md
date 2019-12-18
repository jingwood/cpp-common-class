# cpp-common-class

Common classes for cross-platform C++ application development. (libucm.a)

# This library contains the following classes

- [*ansi.h*](src/ansi.h) Predefined symbols used to output colored information onto console
- [*archive.h*](src/archive.h) File archive
- [*argline.h*](src/argline.h) Functionality for console arguments parsing
- wip...

# Build

Run make to build .a file on each platform.

```shell
cd build/<platform>/
make
```

Need clang++ and c++11 support.

# Add reference in C++ application

1. Include the header files into your application
2. Add `libcmm.a` as input file during link

# License

MIT License

Copyright (c) 2016-2019 Jingwood, unvell.com.
