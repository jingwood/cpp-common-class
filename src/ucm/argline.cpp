///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "argline.h"
#include "lexer.h"

#include <memory>

#if defined(_WIN32)
#include <windows.h>
#endif /* _WIN32 */

namespace ucm {

CommandLineReader::CommandLineReader(const int argc, const char* argv[])
: argc(argc), argv(argv) {
	
}

bool CommandLineReader::hasNextArg() {
	return this->currentIndex < argc;
}

bool CommandLineReader::isArg(const char *arg) {
	const char* input = this->argv[this->currentIndex];
	return strcmp(input, arg) == 0;
}

bool CommandLineReader::nextArg() {
	this->currentIndex++;
	return this->currentIndex < argc;
}

bool CommandLineReader::nextArg(string* str) {
	if (!hasNextArg()) return false;
	
	const char* input = this->argv[this->currentIndex];
	*str = input;
	this->nextArg();
	return true;
}

bool CommandLineReader::nextArg(int* v) {
	if (!nextArg()) return false;
	
	const char* input = this->argv[this->currentIndex];
	
	if (sscanf(input, "%d", v) == 1) {
		this->nextArg();
		return true;
	}
	
	this->nextArg();
	return false;
}

}