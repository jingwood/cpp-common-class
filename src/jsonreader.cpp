///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "jsonreader.h"

#define COLON ':'
#define COMMA ','
#define LCBRACKET '{'
#define RCBRACKET '}'

JSONReader::JSONReader(const string& str) {
	this->init(str);
}

void JSONReader::init(const string& str) {
	this->lexer.setInput(str);
}

JSObject* JSONReader::readObject() {
  if (!this->lexer.readChar(LCBRACKET)) {
    return NULL;
  }
  
  JSObject* object = new JSObject();

  while (true) {
		string key;
    
    if (this->readKey(&key)) {
			if (!this->lexer.readChar(COLON)) {
				break;
			}
      
      JSValue value;
			if (!readValue(value)) {
				break;
			}
      
			object->setProperty(key, value);
    }
    
    if (this->lexer.readChar(RCBRACKET)) {
      break;
    }

    if (!this->lexer.readChar(COMMA)) {
      break;
    }
  }
  
  return object;
}

const bool JSONReader::readKey(string* key) {
  const string* readkey = NULL;
  
  if (this->lexer.readIdentifier()) {
		readkey = &this->lexer.getTokenInputString();
  }
  else if (this->lexer.readString()) {
		readkey = &this->lexer.getTokenInputStringWithoutQuotations();
  }

	if (readkey != NULL) {
		*key = *readkey;
		return true;
	}

  return false;
}

bool JSONReader::readValue(JSValue& value) {
  JSObject* obj = NULL;
  std::vector<JSValue>* list = NULL;
  
  // string
  if (this->lexer.readString()) {
		value.str = new string(lexer.getTokenInputStringWithoutQuotations());
    value.type = JSType::JSType_String;
    return true;
  }
  // number
  else if (this->lexer.readNumber()) {
    value.number = this->lexer.getCurrentToken().v_num;
    value.type = JSType::JSType_Number;
    return true;
  }
	else if (this->lexer.readBoolean()) {
		value.boolean = this->lexer.getCurrentToken().v_bool;
		value.type = JSType::JSType_Boolean;
		return true;
	}
  // identifier
  else if (this->lexer.readIdentifier()) {
    value.str = new string(this->lexer.getTokenInputString());
    value.type = JSType::JSType_Identifier;
    return true;
  }
  // object
  else if ((obj = this->readObject()) != NULL) {
    value.object = obj;
    value.type = JSType::JSType_Object;
    return true;
  }
  else if (this->readArray(&list)) {
    value.array = list;
    value.type = JSType::JSType_Array;
    return true;
  }
  else
    return false;
}

bool JSONReader::readArray(std::vector<JSValue>** list) {
  if (!this->lexer.readChar('[')) {
    return false;
  }
  
  while (true) {
    JSValue value;
    if (!this->readValue(value)) break;
    
    if (*list == NULL) {
      *list = new std::vector<JSValue>();
    }
    
    (*list)->push_back(value);
    
    if (!lexer.readChar(COMMA)) break;
  }
  
  return this->lexer.readChar(']');
}
