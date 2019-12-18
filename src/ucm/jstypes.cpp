///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "jstypes.h"

namespace unvell_common_class {

JSObject::~JSObject() {
	for (auto& p : this->properties) {
		if (p.second.type == JSType::JSType_String) {
			if (p.second.str != NULL) {
				delete p.second.str;
				p.second.str = NULL;
			}
		}
		else if (p.second.type == JSType::JSType_Object) {
			if (p.second.object != NULL) {
				delete p.second.object;
				p.second.object = NULL;
			}
		}
		else if (p.second.type == JSType::JSType_Array) {
			if (p.second.array != NULL) {
				delete p.second.array;
				p.second.array = NULL;
			}
		}
	}
	
	this->properties.clear();
}

void JSObject::setProperty(const string& key, JSValue value) {
	this->properties[key] = value;
}

void JSObject::setProperty(const char* key, JSValue value) {
  this->properties[key] = value;
}

void JSObject::setPropertyFormat(const string& key, const char* format, ...) {
	string str;
	
	va_list vargs;
	va_start(vargs, format);
	str.appendFormat(format, vargs);
	va_end(vargs);
	
	this->setProperty(key, str);
}

//void JSObject::setProperty(const char* key, const string& str) {
//	this->properties[key] = JSValue(str);
//}

//void JSObject::setProperty(const string& key, JSObject* obj) {
//	this->properties[key] = JSValue(obj);
//}

template<typename T>
void setProperty(const string& key, std::vector<T>& array) {
	
}

bool JSObject::hasProperty(const char* key, const JSType type) const {
	const auto& it = this->properties.find(key);
	
	if (it == this->properties.end()) {
		return false;
	}
	
	if (type != JSType::JSType_Unknown && it->second.type != type)
		return false;
	
	if (it->second.type == JSType::JSType_String && it->second.str == NULL)
		return false;
	
	if (it->second.type == JSType::JSType_Object && it->second.object == NULL)
		return false;
	
	return true;
}

JSValue JSObject::getProperty(const char* key, const JSType requireType) const {
  const auto& it = this->properties.find(key);
  
  if (it != this->properties.end()) {
    if (requireType == JSType_Unknown || it->second.type == requireType) {
      return it->second;
    }
  }
  
  return JSValue();
}

double JSObject::getNumberProperty(const char* key, const double defValue) const {
	const JSValue& val = this->getProperty(key, JSType::JSType_Number);
	return (val.type == JSType::JSType_Number) ? val.number : defValue;
}

string* JSObject::getStringProperty(const char* key) const {
	const JSValue& val = this->getProperty(key, JSType::JSType_String);
	return (val.type == JSType::JSType_String) ? val.str : NULL;
}

std::vector<JSValue>* JSObject::getArrayProperty(const char* key) const {
	const JSValue& val = this->getProperty(key, JSType::JSType_Array);
	return (val.type == JSType::JSType_Array) ? val.array : NULL;
}

JSObject* JSObject::getObjectProperty(const char* key) const {
	const JSValue& val = this->getProperty(key, JSType::JSType_Object);
	return (val.type == JSType::JSType_Object) ? val.object : NULL;
}

bool JSObject::isBooleanPropertyTrue(const char *key) const {
	const JSValue& val = this->getProperty(key, JSType::JSType_Boolean);
	return val.type == JSType_Boolean && val.boolean;
}

bool JSObject::isBooleanPropertyFalse(const char* key) const {
	const JSValue& val = this->getProperty(key, JSType::JSType_Boolean);
	return val.type == JSType_Boolean && !val.boolean;
}

}