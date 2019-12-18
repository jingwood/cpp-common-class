///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef exception_h
#define exception_h

#include <stdio.h>

#include "string.h"

namespace unvell_common_class {

class Exception {
private:
  const string message;
  
public:
	Exception() { }
  Exception(const string& msg) : message(msg) { }
  
  const char* getMessage();
};

class ArgumentOutOfRangeException : public Exception {
};

class BufferUnavailableException : public Exception {
};

}

#endif /* exception_h */
