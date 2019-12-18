///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "jsonwriter.h"

namespace unvell_common_class {

JSONOutputFormat JSONOutputFormat::defaultFormat;

JSONOutputFormat::JSONOutputFormat() {
	static bool predefineFormatInited = false;
	
	if (!predefineFormatInited) {
		predefineFormatInited = true;
	}
}

const string& JSONWriter::getString() const {
	return this->sb;
}

void JSONWriter::reset() {
	this->sb.clear();
}

void JSONWriter::pushScopeStack(WritingScopeType type) {
	WritingScope scope(type);
	if (this->scopeStack.size() > 0) {
		scope.indents = this->scopeStack.top().indents + 1;
	}
	this->scopeStack.push(scope);
}

void JSONWriter::popScopeStack() {
	this->scopeStack.pop();
}

void JSONWriter::appendString(const string& str) {
	this->sb.append(str);
}

void JSONWriter::appendString(const char* format, ...) {
	va_list vargs;
	va_start(vargs, format);
	this->appendString(format, vargs);
	va_end(vargs);
}

void JSONWriter::appendString(const char* format, va_list vargs) {
	this->sb.appendFormat(format, vargs);
}

void JSONWriter::appendColon() {
	if (this->format.spaceBeforeColon) sb.append(' ');
	this->sb.append(':');
	if (this->format.spaceAfterColon) sb.append(' ');
}

void JSONWriter::appendSeparatorComma() {
	if (this->currentObjectScope().firstProperty) {
		this->currentObjectScope().firstProperty = false;
	} else {
		this->sb.append(',');
		
		bool newline = false;
		
		if (this->scopeStack.top().type == WritingScopeType::WST_Object) {
			newline = this->format.newlineAfterProperty;
		} else if (this->scopeStack.top().type == WritingScopeType::WST_Array) {
			newline = this->format.newlineAfterArrayElement;
		}
		
		if (newline) {
			this->sb.append(JSON_NEWLINE);
			this->appendIndents();
		} else if (this->format.spaceAfterComma) {
			this->sb.append(' ');
		}
	}
}

void JSONWriter::appendObjectBegin() {
	this->sb.append('{');
	if (this->format.newlineAfterObjectBegin) {
		this->sb.append(JSON_NEWLINE);
		this->appendIndents();
	}
}

void JSONWriter::appendObjectEnd() {
	if (this->format.newlineBeforeObjectEnd) {
		this->sb.append(JSON_NEWLINE);
		this->appendIndents();
	}
	this->sb.append('}');
}

void JSONWriter::appendArrayBegin() {
	this->sb.append('[');
	if (this->format.newlineAfterArrayBegin) {
		this->sb.append(JSON_NEWLINE);
		this->appendIndents();
	}
}

void JSONWriter::appendArrayEnd() {
	if (this->format.newlineBeforeArrayEnd) {
		this->sb.append(JSON_NEWLINE);
		this->appendIndents();
	}
	this->sb.append(']');
}

void JSONWriter::appendIndents() {
	if (!this->format.indent) return;
	
	int indents = 0;
	
	if (this->scopeStack.size() > 0) {
		const WritingScope& scope = this->scopeStack.top();
		indents = scope.indents * this->format.indentSpaces;
	}
	
	for (int i = 0; i < indents; i++) {
		this->sb.append(' ');
	}
}

void JSONWriter::appendPropertyKey(const char* key) {
	this->appendSeparatorComma();
	
	if (this->format.doubleQuoteKey) {
		this->sb.append('"');
	}
	this->appendString(key);
	if (this->format.doubleQuoteKey) {
		this->sb.append('"');
	}
	this->appendColon();
}

void JSONWriter::beginObjectWithKey(const string& key) {
	this->appendPropertyKey(key);
	this->beginObject();
}

void JSONWriter::beginObjectWithKey(const char* key) {
	this->appendPropertyKey(key);
	this->beginObject();
}

void JSONWriter::beginObject() {
	this->pushScopeStack(WritingScopeType::WST_Object);
	this->appendObjectBegin();
}

void JSONWriter::endObject() {
	this->popScopeStack();
	this->appendObjectEnd();
}

void JSONWriter::beginArrayWithKey(const string& key) {
	this->appendPropertyKey(key);
	this->beginArray();
}

void JSONWriter::beginArray() {
	this->pushScopeStack(WritingScopeType::WST_Array);
	this->appendArrayBegin();
}

void JSONWriter::endArray() {
	this->popScopeStack();
	this->appendArrayEnd();
}

void JSONWriter::writeArrayElement(const string& value) {
	this->appendSeparatorComma();
	this->appendString(value);
}

void JSONWriter::writeArrayElement(const char* format, ...) {
	this->appendSeparatorComma();
	
	va_list vargs;
	va_start(vargs, format);
	this->writeStringFormat(format, vargs);
	va_end(vargs);
}

void JSONWriter::writeArrayElement(const double value) {
	this->appendSeparatorComma();
	this->writeNumber(value);
}

void JSONWriter::writeArrayElementString(const string& str) {
	this->appendSeparatorComma();
	this->writeString(str);
}

void JSONWriter::writeProperty(const char* key, const string& str) {
	this->appendPropertyKey(key);
	this->writeString(str);
}

void JSONWriter::writeProperty(const char* key, const int val) {
	this->appendPropertyKey(key);
	this->writeNumber(val);
}

void JSONWriter::writeProperty(const char* key, const double val) {
	this->appendPropertyKey(key);
	this->writeNumber(val);
}

void JSONWriter::writeProperty(const char* key, const bool val) {
	this->appendPropertyKey(key);
	this->writeBoolean(val);
}

void JSONWriter::writeProperty(const char* key, const JSObject& obj) {
	this->appendPropertyKey(key);
	this->writeObject(obj);
}

void JSONWriter::writeProperty(const char* key, const JSValue& val) {
	if (val.type != JSType::JSType_Unknown) {
		this->appendPropertyKey(key);
		this->writeValue(val);
	}
}

void JSONWriter::writeProperty(const char* key, const char* format, ...) {
	this->appendPropertyKey(key);
	
	va_list vargs;
	va_start(vargs, format);
	this->writeStringFormat(format, vargs);
	va_end(vargs);
}

void JSONWriter::writeCustomProperty(const char* key, const string& value) {
	this->appendPropertyKey(key);
	this->appendString(value);
}

void JSONWriter::writeCustomProperty(const char* key, const char* format, ...) {
	this->appendPropertyKey(key);
	
	va_list vargs;
	va_start(vargs, format);
	this->appendString(format, vargs);
	va_end(vargs);
}

void JSONWriter::writeValue(const JSValue& value) {
	switch (value.type) {
		default:
			break;
			
		case JSType::JSType_Number:
			this->writeNumber(value.number);
			break;
			
		case JSType::JSType_String:
			this->sb.append('"');
			this->appendString(*value.str);
			this->sb.append('"');
			break;
			
		case JSType::JSType_Array:
			this->writeArray(*value.array);
			break;
			
		case JSType::JSType_Boolean:
			this->writeBoolean(value.boolean);
			break;
			
		case JSType::JSType_Object:
			this->writeObject(*value.object);
			break;
	}
}

void JSONWriter::writeValue(const string& value) {
	this->appendString(value);
}

void JSONWriter::writeNumber(const int value) {
	this->sb.appendFormat("%d", value);
}

void JSONWriter::writeNumber(const double value) {
	string str;
	str.appendFormat("%f", value);
	
	if (str.lastIndexOf('.') > 0) {
		while (str.endsWith('0')) {
			str.remove(str.length() - 1, 1);
		}
		
		if (str.endsWith('.')) {
			str.remove(str.length() - 1, 1);
		}
	}

	this->sb.append(str);
}

void JSONWriter::writeBoolean(const bool value) {
	if (value) {
		this->sb.append("true");
	} else {
		this->sb.append("false");
	}
}

void JSONWriter::writeString(const string& str) {
	this->sb.append('"');
	this->appendString(str);
	this->sb.append('"');
}

void JSONWriter::writeStringFormat(const char* format, ...) {
	va_list vargs;
	va_start(vargs, format);
	this->writeStringFormat(format, vargs);
	va_end(vargs);
}

void JSONWriter::writeStringFormat(const char* format, va_list vargs) {
	this->sb.append('"');
	this->appendString(format, vargs);
	this->sb.append('"');
}

void JSONWriter::writeArray(const std::vector<JSValue>& array) {
	this->appendArrayBegin();
	this->pushScopeStack(WritingScopeType::WST_Array);
	
	for (const auto& i : array) {
		this->appendSeparatorComma();
		this->writeValue(i);
	}
	
	this->popScopeStack();
	this->appendArrayEnd();
}

void JSONWriter::writeObject(const JSObject& obj) {
	this->beginObject();
	
	for (const auto& p : obj.getProperties()) {
		this->writeProperty(p.first, p.second);
	}
	
	this->endObject();
}

void JSONWriter::convertToJSON(const JSObject &obj, string &str) {
	JSONWriter writer;
	writer.writeObject(obj);
	
	str.expand(writer.sb.length());
	str.append(writer.sb);
}

}