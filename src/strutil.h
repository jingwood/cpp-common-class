///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef strutility_h
#define strutility_h

#include <stdio.h>
#include <memory>

#include "string.h"

#define   substr(dest, src, pos, len)   memcpy(dest, src + pos, len)

namespace ccm {
	
inline unsigned int hash(const char* s, unsigned int seed = 0) {
  unsigned int hash = seed;
  
  while (*s) {
    hash = (hash * 101 + *s++) & 0xFFFFFFFF;
  }
  
  return hash;
}

int hex2dec(const char* str, const int len);

class CommandLineArgumentReader {
private:
	int argc;
	const char** args;
	int currentIndex;
	
public:
	CommandLineArgumentReader(int argc, const char** args)
	: argc(argc), args(args) {
	}
	
	bool nextArgument(string& arg);
};

void formatFriendlyDate(const double seconds, string& str);

}

#endif /* strutility_h */
