///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef jsonwriter_h
#define jsonwriter_h

#include <stdio.h>
#include <stack>

#include "string.h"
#include "types.h"
#include "jstypes.h"

#define JSON_NEWLINE '\n'

namespace unvell_common_class {

struct JSONOutputFormat {
	bool indent = true;
	uint indentSpaces = 2;
	bool doubleQuoteKey = true;
	bool newlineAfterObjectBegin = true;
	bool newlineBeforeObjectEnd = true;
	bool newlineAfterArrayBegin = false;
	bool newlineBeforeArrayEnd = false;
	bool newlineAfterProperty = true;
	bool newlineAfterArrayElement = false;
	bool spaceAfterComma = true;
	bool spaceBeforeColon = false;
	bool spaceAfterColon = true;

	JSONOutputFormat();

	static JSONOutputFormat defaultFormat;
//	static JSONOutputFormat compactFormat;
//	static JSONOutputFormat comfortableFormat;
};

enum WritingScopeType {
	WST_Object,
	WST_Array,
};

struct WritingScope {
	WritingScopeType type = WST_Object;
	bool firstProperty = true;
	int indents = 0;
	
	WritingScope(WritingScopeType type) : type(type) { }
};

class JSONWriter
{
private:
	string sb;
	std::stack<WritingScope> scopeStack;

	void pushScopeStack(WritingScopeType type);
	void popScopeStack();
	
	inline WritingScope& currentObjectScope() {
		return this->scopeStack.top();
	}
	
	void appendString(const string& str);
	void appendString(const char* format, ...);
	void appendString(const char* format, va_list vargs);

	void appendColon();
	void appendSeparatorComma();
	
	void appendObjectBegin();
	void appendObjectEnd();
	void appendArrayBegin();
	void appendArrayEnd();
	void appendIndents();
	void appendPropertyKey(const char* key);
	
	void writeValue(const JSValue& value);
	void writeValue(const string& value);
	void writeNumber(const int value);
	void writeNumber(const double value);
	void writeBoolean(const bool value);
	void writeString(const string& str);
	void writeStringFormat(const char* format, ...);
	void writeStringFormat(const char* format, va_list vargs);
	void writeArray(const std::vector<JSValue>& array);

public:
	JSONOutputFormat format;
	
	JSONWriter() : sb(512) { }
	const string& getString() const;
	
	void reset();
	
	void beginObjectWithKey(const string& key);
	void beginObjectWithKey(const char* key);
	void beginObject();
	void endObject();
	void beginArrayWithKey(const string& key);
	void beginArray();
	void endArray();

	void writeObject(const JSObject& obj);
	
	void writeArrayElement(const string& value);
	void writeArrayElement(const char* format, ...);
	void writeArrayElement(const double num);
	void writeArrayElementString(const string& str);
	
	void writeProperty(const char* key, const int num);
	void writeProperty(const char* key, const double num);
	void writeProperty(const char* key, const bool value);
	void writeProperty(const char* key, const JSObject& obj);
	void writeProperty(const char* key, const string& value);
	void writeProperty(const char* key, const JSValue& val);
	void writeProperty(const char* key, const char* format, ...);
	void writeCustomProperty(const char* key, const string& value);
	void writeCustomProperty(const char* key, const char* format, ...);

	static void convertToJSON(const JSObject& obj, string& str);
};

}

#endif /* jsonwriter_h */
