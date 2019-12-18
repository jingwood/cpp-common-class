///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "strutil.h"
#include <math.h>

namespace unvell_common_class {

int hex2dec(const char* str, const int len) {
	char* c = const_cast<char*>(str) + len - 1;

	int value = 0;
	int digit = 0;

	for (int i = 0; i < len; i++) {
		char d = *c;

		if (d >= '0' && d <= '9') {
			value += (d - '0') * (int)pow(16, digit);
		}
		else if (d >= 'A' && d <= 'F') {
			value += (d - 'A' + 10) * (int)pow(16, digit);
		}
		else if (d >= 'a' && d <= 'f') {
			value += (d - 'a' + 10) * (int)pow(16, digit);
		}

		c--;
		digit++;
	}

	return value;
}

bool CommandLineArgumentReader::nextArgument(string& arg) {
	if (this->currentIndex < this->argc) {
		arg = this->args[this->currentIndex];
		
		this->currentIndex++;
		return true;
	} else {
		return false;
	}
}

void formatFriendlyDate(const double seconds, string& str) {
	str.clear();
	
	long utime = (long)(seconds + 0.5f);
	
	if (utime > 60*60) {
		str.appendFormat("%dh", utime / 60 / 60);
		utime %= 60*60;
	}
	
	if (utime > 60) {
		if (!str.isEmpty()) str.append(' ');
		str.appendFormat("%dm", utime / 60);
		utime %= 60;
	}
	
	if (!str.isEmpty()) str.append(' ');
	str.appendFormat("%ds", utime);
}

}
