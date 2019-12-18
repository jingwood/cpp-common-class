///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "stringstream.h"

namespace unvell_common_class {

	StringReader::StringReader(const string& str) {
		this->setInput(str);
	}

	void StringReader::setInput(const string& str) {
		this->input = str;
		this->pos = 0;
	}

	char StringReader::readChar() {
		if (this->isEnd()) {
			return STR_EOF;
		}

		return this->input[pos++];
	}
}
