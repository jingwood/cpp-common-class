# Common classes module (libucm.a)

Common classes for cross-platform C++ application development. 

# Classes

The following classes are included in this library.

- [*ansi.h*](src/ucm/ansi.h) Predefined symbols used to output colored information onto console
- [*archive.h*](src/ucm/archive.h) File archive
- [*argline.h*](src/ucm/argline.h) Functionality for console arguments parsing
- [*console.h*](src/ucm/console.h) Standard console input/output wrapper class
- [*deflate.h*](src/ucm/deflate.h) Compressing functionality based on libz (zlib)
- [*file.h*](src/ucm/file.h) File access APIs
- [*filestream.h*](src/ucm/filestream.h) Stream to read and write into file
- [*jsonreader.h*](src/ucm/jsonreader.h) JSON format reader
- [*jsonwriter.h*](src/ucm/jsonwriter.h) JSON format writter
- [*jstypes.h*](src/ucm/jstypes.h) JSON type defines
- [*lexer.h*](src/ucm/lexer.h) Lexer for parsing JSON format
- [*stopwatch.h*](src/ucm/stopwatch.h) Stopwatch for elapsed time count
- wip...

# Build

Run make to build .a file on each platform.

```shell
cd build/<platform>/
make
```

Need clang++ and c++11 support.

# Add reference in C++ application

1. Include the header files into target application
2. Add `libucm.a` as link input file

# License

MIT License

Copyright (c) 2016-2019 Jingwood, unvell.com, All rights reserved.
