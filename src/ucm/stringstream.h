///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef stringstream_h
#define stringstream_h

#include <stdio.h>

#include "string.h"

namespace unvell_common_class {
	
	class StringReader
	{
	private:
		int pos = 0;
		string input;
		
	public:
		StringReader() { }
		StringReader(const string& str);
		
		void setInput(const string& str);
		
		inline const string& getInput() const { return this->input; }
		
		char readChar();
		
		inline const int getPosition() const { return this->pos; }
		inline void setPosition(int pos) { this->pos = pos; }
		
		inline const int getLength() const { return this->input.length(); }

		inline bool isEnd() const { return this->pos >= this->input.length(); }
	};
	
}

#endif /* stringstream_h */
