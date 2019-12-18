///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef jstypes_h
#define jstypes_h

#include <stdio.h>
#include <vector>
#include <map>

#include "string.h"

#if defined(_WIN32)
#define sscanf sscanf_s
#endif /* _WIN32 */

namespace unvell_common_class {

enum JSType
{
  JSType_Unknown = 0,
  JSType_Number,
  JSType_String,
	JSType_Boolean,
  JSType_Identifier,
  JSType_Object,
  JSType_Array,
};

struct JSValue;

class JSObject {
private:
  std::map<string, JSValue> properties;
  
public:
	~JSObject();

	inline int getPropertyCount() const {
		return (int)this->properties.size();
	}

	void setProperty(const string& key, JSValue value);
	void setProperty(const char* key, JSValue value);
	void setPropertyFormat(const string& key, const char* format, ...);
//	void setProperty(const string& key, JSObject* value);
//	void setProperty(const char* key, const string& str);
	template<typename T>
	void setProperty(const string& key, std::vector<T>& array);
//	void setProperty(const string& key, void* arr, uint length);

	bool hasProperty(const char* key, const JSType type = JSType::JSType_Unknown) const;
  JSValue getProperty(const char* key, const JSType requireType = JSType_Unknown) const;
	
	double getNumberProperty(const char* key, const double defValue = 0) const;
	template<typename T>
	bool tryGetNumberProperty(const char* key, T* value, const bool paraseFromString = false) const;
	
	string* getStringProperty(const char* key) const;
	std::vector<JSValue>* getArrayProperty(const char* key) const;
	JSObject* getObjectProperty(const char* key) const;
	bool isBooleanPropertyTrue(const char* key) const;
	bool isBooleanPropertyFalse(const char* key) const;

  inline const std::map<string, JSValue>& getProperties() const {
    return this->properties;
  }
};

struct JSValue
{
  JSType type = JSType::JSType_Unknown;
  
  union {
    double number = 0;
    string* str;
		bool boolean;
    JSObject* object;
    std::vector<JSValue>* array;
    void* _data;
  };
	
	JSValue(const JSType type = JSType::JSType_Unknown)
	: type(type) {
	}
	
	JSValue(bool b)
	: type(JSType::JSType_Boolean), boolean(b) {
	}
	
	JSValue(int num)
	: type(JSType::JSType_Number), number(num) {
	}
	
	JSValue(double num)
	: type(JSType::JSType_Number), number(num) {
	}
	
	JSValue(string& val)
	: type(JSType::JSType_String) {
		this->str = new string(val.length());
		this->str->append(val);
	}
	
	JSValue(const string& str)
	: type(JSType::JSType_String) {
		this->str = new string(str.length());
		this->str->append(str);
	}
		
	JSValue(std::vector<JSValue>* arr)
	: type(JSType::JSType_Array), array(arr) {
	}
	
	JSValue(JSObject* obj)
	: type(JSType::JSType_Object), object(obj) {
	}
};

template<typename T>
bool JSObject::tryGetNumberProperty(const char* key, T* value, const bool parseFromString) const {
	const JSValue& val = this->getProperty(key, JSType::JSType_Number);
	
	if (val.type == JSType::JSType_Number) {
		*value = (T)val.number;
		return true;
	}

	if (parseFromString && val.type == JSType::JSType_String && val.str != NULL) {
		double num = 0.0;
		
		if (sscanf(val.str->getBuffer(), "%lf", &num) >= 1) {
			*value = (T)num;
			return true;
		}
	}
	
	return false;
}

}

#endif /* jstypes_h */
