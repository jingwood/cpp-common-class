///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef stopwatch_h
#define stopwatch_h

#include <stdio.h>
#include "types.h"

#if !defined(_WIN32)
#include <sys/time.h>
#endif /* _WIN32 */

#if defined(_WIN32)
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64
int gettimeofday(timeval* tp, struct timezone* tzp);
#endif /* defined(_Win32) */

namespace ucm {

class Stopwatch {
private:
	timeval startTime;
	timeval endTime;
	double elapsedSeconds = 0;
	bool running = false;
	
	void refreshElapsedSeconds(const timeval& tv);
	
public:
	void start();
	void stop();
	
	static Stopwatch startNew();

	double getElapsedSeconds();
};

}

#endif /* stopwatch_h */
