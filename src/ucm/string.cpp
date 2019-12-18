///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "string.h"
#include "exception.h"

#include <memory>

namespace unvell_common_class {

#ifdef _WIN32
#define _sprintf sprintf_s
#else
#define _sprintf sprintf
#endif // _WIN32

#define INITIAL_CAPACITY 64
#define EXPAND_INCREASE_SIZE 64

inline bool _char_compare(const char a, const char b, StringComparingFlags flags) {
	if (a != b) {
		if (flags & StringComparingFlags::SCF_CASE_INSENSITIVE) {
			if (islower(a)) {
				if (toupper(a) != b) return false;
			} else if (isupper(a)) {
				if (tolower(a) != b) return false;
			}
		} else {
			return false;
		}
	}
	
	return true;
}

bool stringStartWith(const char* str, int slen, const char* ptarget, int targetlen) {
#if defined(_WIN32) || defined(__APPLE__)
	slen = (int)strnlen(str, slen);
	targetlen = (int)strnlen(ptarget, targetlen);
#else
	slen = (int)strlen(str);
	targetlen = (int)strlen(ptarget);
#endif

	if (targetlen == 0 || targetlen > slen) return false;

	const char* pstr = str;

	for (int i = 0; i < targetlen; i++, pstr++, ptarget++) {
		if (*pstr != *ptarget) {
			return false;
		}
	}

	return true;
}

bool stringEndWith(const char* str, int slen, const char* end, int elen, StringComparingFlags flags) {
#if defined(_WIN32) || defined(__APPLE__)
	slen = (int)strnlen(str, slen);
	elen = (int)strnlen(end, elen);
#else
	slen = (int)strlen(str);
	elen = (int)strlen(end);
#endif

	if (elen == 0 || elen > slen) return false;

	const char* pstr = str + slen;
	const char* pend = end + elen;

	for (int i = 0; i < elen; i++, pstr--, pend--) {
		if (!_char_compare(*pstr, *pend, flags)) return false;
	}
	
	return true;
}

const char string::empty[] = { STR_EOF };

string::string(const int initCapacity) {
	this->expand(initCapacity);
}

string::string(const char* str)
: string(str, (int)strlen(str)) {
}

string::string(const string& str) {
	this->initBuffer(str.len);
	this->append(str);
}

string::string(const char* str, const int len) {
	this->initBuffer(len);
	this->append(str, len);
}

string::~string() {
	if (this->buffer != NULL) {
		delete[] this->buffer;
		this->buffer = NULL;
	}
	this->len = 0;
	this->capacity = 0;
}

void string::initBuffer(const int size) {
	this->buffer = new char[size + 1];
	this->capacity = size + 1;
}

void string::clear() {
	this->len = 0;
	this->buffer[0] = STR_EOF;
}

void string::reset() {
	string::~string();
	this->initBuffer(INITIAL_CAPACITY);
	this->buffer[0] = STR_EOF;
}

void string::expand(const int size, const int copyoffset) {
	if (size < this->capacity) return;
	
	const int m = size % EXPAND_INCREASE_SIZE;
	
	int newCapacity = size;
	
	if (m == 0) {
		if (this->capacity > 0) {
			newCapacity += EXPAND_INCREASE_SIZE;
		}
	} else {
		newCapacity += (EXPAND_INCREASE_SIZE - m);
	}
	
  if (this->buffer == NULL) {
    this->buffer = new char[newCapacity];
    this->capacity = newCapacity;
  } else {
		if (this->capacity < newCapacity) {
			char* newBuffer = new char[newCapacity];
			memcpy(newBuffer + copyoffset, this->buffer, this->len + 1);
			delete[] this->buffer;
			this->buffer = newBuffer;
			this->capacity = newCapacity;
		}
  }
}

void string::append(const char ch) {
  this->expand(this->len + 1);
	
  this->buffer[this->len] = ch;
  this->len++;
  
  this->buffer[this->len] = STR_EOF;
}

void string::append(const char* str) {
  this->append(str, (int)strlen(str));
}

void string::append(const char* str, const int strlen) {
	if (strlen <= 0) return;
	
	this->expand(this->len + strlen);
	
  memcpy(this->buffer + this->len, str, strlen);
  
  this->len += strlen;
  
  this->buffer[this->len] = STR_EOF;
}

void string::append(const string &str) {
	this->append(str.getBuffer(), str.length());
}

void string::append(const string* str) {
	this->append(str->getBuffer(), str->length());
}

void string::appendFormat(const char* format, ...) {
	va_list vargs;
	va_start(vargs, format);
	this->appendFormat(format, vargs);
	va_end(vargs);
}

void string::appendFormat(const char* format, va_list vargs) {
	// FIXME: measure string length
	char tmpstr[1024];
	vsnprintf(tmpstr, 1024, format, vargs);
	this->append(tmpstr);
}

void string::appendLine(const char* line)
{
  this->append(line);
  this->append(NEW_LINE);
}

void string::insert(const int index, const char* str) {
	this->insert(index, str, (int)strlen(str));
}

void string::insert(const int index, const char* str, const int strlen) {
	int newlen = this->len + strlen + 1;

	if (newlen > capacity) {
		expand(newlen, strlen);
	} else {
		memmove(this->buffer + index + strlen, this->buffer + index, this->len - index + 1);
	}

	memcpy(this->buffer + index, str, strlen);

	this->len += strlen;
}

void string::remove(const int index, int len) {
	if (index + len > this->len) {
		len = this->len - index;
	}
	
	const int moveLen = this->length() - index - len + 1;
	
	memmove(this->buffer + index, this->buffer + index + len, moveLen);
	
	this->len -= len;
}

string& string::replace(const char from, const char to) {
	for (char* c = this->buffer; c < this->buffer + this->len; c++) {
		if (*c == from) *c = to;
	}
	return *this;
}

char string::charAt(const uint index) const {
	if (index >= this->len) throw ArgumentOutOfRangeException();
	return this->buffer[index];
}

int string::indexOf(const char c) const {
	return (int)(size_t)(strchr(this->buffer, c) - this->buffer);
}

int string::indexOf(const string& str, const int startIndex) const {
	if (startIndex >= this->length()) return -1;
	
	const char* p = this->buffer;
	for (int i = startIndex; i < this->length(); i++, p++) {
		bool matched = true;
		
		const char* q = str.buffer;
		for (int k = 0; k < str.length(); k++, q++)
			if (*p != *q) {
				matched = false;
				break;
			}
		
		if (matched) {
			return i;
		}
	}
	
	return -1;
}

int string::lastIndexOf(const char c) const {
	const char* ch = strrchr(this->buffer, c);
	return ch == NULL ? -1 : (int)(size_t)(ch - this->buffer);
}

bool string::startsWith(const char c) const {
	return this->len > 0 && this->buffer[0] == c;
}

bool string::startsWith(const string& str) const {
	return this->startsWith(str.buffer, str.len);
}

bool string::startsWith(const char* str) const {
	return this->startsWith(str, (int)strlen(str));
}

bool string::startsWith(const char* str, const int strlen) const {
	return stringStartWith(this->buffer, this->len, str, strlen);
}

bool string::endsWith(const char c) const {
	return this->len > 0 && this->buffer[this->len - 1] == c;
}

bool string::endsWith(const string& str, StringComparingFlags flags) const {
  return this->endsWith(str.buffer, str.len, flags);
}

bool string::endsWith(const char* str, StringComparingFlags flags) const {
	return this->endsWith(str, (int)strlen(str), flags);
}

bool string::endsWith(const char* str, const int strlen, StringComparingFlags flags) const {
	return stringEndWith(this->buffer, this->len, str, strlen, flags);
}

void string::substring(const unsigned int start, string& substr) const {
	if (start >= this->length()) {
		throw ArgumentOutOfRangeException();
	}
	
	this->substring(start, this->length() - start, substr);
}

void string::substring(const unsigned int start, const unsigned int length, string& substr) const {
	if (start + length > this->length()) {
		throw ArgumentOutOfRangeException();
	}
	
	substr.append(this->buffer + start, length);
}

bool string::contains(const string& str) const {
	return strstr(this->getBuffer(), str) != NULL;
}

bool string::operator<(const string& str) const {
  return strcmp(this->buffer, str.buffer) < 0;
}

char string::operator[](const int index) const {
	return this->buffer[index];
}

void string::operator=(const char* str) {
	this->clear();
	this->append(str);
}

void string::operator=(const string& str) {
	this->clear();
	this->append(str);
}

string string::operator+(const char* str) {
	string tmp = *this;
	tmp.append(str);
	return tmp;
}

string& string::operator+=(const char c) {
	this->append(c);
	return *this;
}

string& string::operator+=(const char* str) {
	this->append(str);
	return *this;
}

string& string::operator+=(const string& str) {
	this->append(str);
	return *this;
}

string::operator const char* () const {
	return this->buffer;
}

bool string::equals(const char* str) const {
	size_t slen = strlen(str);
	
	if (this->len != slen) return false;
	if (this->len == 0 && slen == 0) return true;
	
	return strcmp(this->buffer, str) == 0;
}

bool string::equals(const string& str) const {
	return this->len == str.len && this->equals(str.buffer);
}

bool string::operator==(const char* str) const {
	return this->equals(str);
}

bool string::operator==(const string& str) const {
	return this->equals(str);
}

bool string::operator!=(const char* str) const {
	return !this->equals(str);
}

bool string::operator!=(const string& str) const {
	return !this->equals(str);
}

bool string::isEmpty() const {
	return this->length() <= 0;
}

//string operator "" _s(const char* str, unsigned long len) { return string(str, (int)len); }

bool string::isNullOrEmpty(const string* str) {
	return (str == NULL || str->isEmpty());
}

//string string::format(const char *format, ...) {
//	string str;
//	va_list vargs;
//	va_start(vargs, format);
//	str.appendFormat(format, vargs);
//	va_end(vargs);
//	return str;
//}

void string::decode(const byte* buf, const size_t length, string* str) {
	if (str == NULL) return;
	str->clear();
	
	const byte* b = buf;
	for (size_t i = 0; i < length; i++, b++) {
		str->append((char)*b);
	}
}

void string::encode(const string &str, byte** buf, size_t *length) {
	*length = str.length();
	
	*buf = new byte[*length];
	
	const char* c = str.getBuffer();
	byte* bb = *buf;
	for (size_t i = 0; i < *length; i++, c++, bb++) {
		*bb = (byte)*c;
	}
}

bool string::compare(const char* str1, const char* str2, StringComparingFlags flags) {
	for (const char *pa = str1, *pb = str2; *pa != STR_EOF && *pb != STR_EOF; pa++, pb++) {
		if (!_char_compare(*pa, *pb, flags)) return false;
	}
	
	return true;
}

}
