///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32)
#pragma once
#endif /* _WIN32 */

#ifndef argline_h
#define argline_h

#include "string.h"

namespace ucm {

class CommandLineReader {
private:
	int argc = 0;
	const char** argv = NULL;
	int currentIndex = 1;
	
public:
	CommandLineReader(const int argc, const char* argv[]);
	
	bool hasNextArg();
	bool isArg(const char* arg);

	bool nextArg();
	bool nextArg(string* str);
	bool nextArg(int* v);
};

}

#endif /* argline_h */
