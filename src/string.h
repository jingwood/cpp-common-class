///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef string_h
#define string_h

#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "types.h"

namespace ccm {
	
#define INCREASE_CAPACITY 64
#define STR_EOF '\0'

#if defined(_WIN32)
#define NEW_LINE "\r\n"
#else
#define NEW_LINE "\n"
#endif /* _WIN32 */

enum StringComparingFlags : unsigned int {
	SCF_NONE = 0x0,
	SCF_CASE_INSENSITIVE = 0x1,
};

inline bool isLowerCase(char c) {
	return c >= 97 && c <= 122;
}

inline bool isUpperCase(char c) {
	return c >= 65 && c <= 90;
}

inline char toUpperCase(char c) {
	return c - 32;
}

inline char toLowerCase(char c) {
	return c + 32;
}

bool stringStartWith(const char* str, int strlen, const char* ptarget, int endlen);
bool stringEndWith(const char* str, int strlen, const char* end, int endlen, StringComparingFlags flags = SCF_NONE);

class string
{
private:
  char* buffer = NULL;
  uint len = 0;
  uint capacity = 0;
  
public:
	string(const int initCapacity = INCREASE_CAPACITY);
	string(const char* str);
	string(const char* str, const int len);
	string(const string& str);
  ~string();
	
	static const char empty[];

	void initBuffer(const int size);
  void expand(const int size, const int copyoffset = 0);
 
  void append(const char ch);
  void append(const char* str);
  void append(const char* str, const int strlen);
	void append(const string& str);
	void append(const string* str);

  void appendFormat(const char* format, ...);
	void appendFormat(const char* format, va_list vargs);
  void appendLine(const char* line);

	void insert(const int index, const char* str);
	void insert(const int index, const char* str, const int strlen);
	
	void remove(const int index, int len);

	string& replace(const char from, const char to);
	void replace(const string& from, const string& to);

	bool startsWith(const char c) const;
	bool startsWith(const string& str) const;
	bool startsWith(const char* str) const;
	bool startsWith(const char* str, const int len) const;

	bool endsWith(const char c) const;
	bool endsWith(const string& str, StringComparingFlags flags = SCF_NONE) const;
	bool endsWith(const char* str, StringComparingFlags flags = SCF_NONE) const;
	bool endsWith(const char* str, const int len, StringComparingFlags flags = SCF_NONE) const;

	char charAt(const uint index) const;
  int indexOf(const char c) const;
	int indexOf(const string& str, const int startIndex = 0) const;
  int lastIndexOf(const char c) const;
	
	void substring(const unsigned int start, string& substr) const;
	void substring(const unsigned int start, const unsigned int length, string& substr) const;

	bool contains(const string& str) const;

	void clear();
	void reset();
	
  inline const int length() const { return this->len; }
	inline const char* getBuffer() const { return this->buffer; }
	inline const char* c_str() const { return this->getBuffer(); }
	
  bool operator<(const string& str) const;
	char operator[](const int index) const;
	
	void operator=(const char* str);
	void operator=(const string& str);
	
	string operator+(const char* str);

	string& operator+=(const char c);
	string& operator+=(const char* str);
	string& operator+=(const string& str);
	
	operator const char* () const;
	
	bool equals(const char* str) const;
	bool equals(const string& str) const;
	
	bool operator==(const char* str) const;
	bool operator==(const string& str) const;
	bool operator!=(const char* str) const;
	bool operator!=(const string& str) const;
	
	bool isEmpty() const;
	
	static bool isNullOrEmpty(const string* str);

//	static string format(const char* format, ...);
	
	static void decode(const byte* buf, const size_t length, string* str);
	static void encode(const string& str, byte** buf, size_t* length);
	
	static bool compare(const char* str1, const char* str2, StringComparingFlags flags = StringComparingFlags::SCF_NONE);
};

}

#endif /* string_h */

