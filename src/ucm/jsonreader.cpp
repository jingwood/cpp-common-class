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

namespace ucm {

static inline int hexDigit(char h) {
	if (h >= '0' && h <= '9') return h - '0';
	if (h >= 'a' && h <= 'f') return 10 + (h - 'a');
	if (h >= 'A' && h <= 'F') return 10 + (h - 'A');
	return -1;
}

static void appendUTF8(string& out, unsigned int cp) {
	if (cp < 0x80) {
		out.append((char)cp);
	} else if (cp < 0x800) {
		out.append((char)(0xC0 | (cp >> 6)));
		out.append((char)(0x80 | (cp & 0x3F)));
	} else if (cp < 0x10000) {
		out.append((char)(0xE0 | (cp >> 12)));
		out.append((char)(0x80 | ((cp >> 6) & 0x3F)));
		out.append((char)(0x80 | (cp & 0x3F)));
	} else {
		out.append((char)(0xF0 | (cp >> 18)));
		out.append((char)(0x80 | ((cp >> 12) & 0x3F)));
		out.append((char)(0x80 | ((cp >> 6) & 0x3F)));
		out.append((char)(0x80 | (cp & 0x3F)));
	}
}

void JSONReader::unescapeJSONString(const string& raw, string& out) {
	out.clear();
	const char* s = raw.getBuffer();
	const int n = raw.length();
	for (int i = 0; i < n; i++) {
		if (s[i] != '\\' || i + 1 >= n) {
			out.append(s[i]);
			continue;
		}
		char esc = s[++i];
		switch (esc) {
			case '"':  out.append('"');  break;
			case '\\': out.append('\\'); break;
			case '/':  out.append('/');  break;
			case 'b':  out.append('\b'); break;
			case 'f':  out.append('\f'); break;
			case 'n':  out.append('\n'); break;
			case 'r':  out.append('\r'); break;
			case 't':  out.append('\t'); break;
			case 'u': {
				if (i + 4 >= n) { out.append(esc); break; }
				unsigned int cp = 0;
				bool ok = true;
				for (int j = 1; j <= 4; j++) {
					int v = hexDigit(s[i + j]);
					if (v < 0) { ok = false; break; }
					cp = (cp << 4) | (unsigned int)v;
				}
				if (!ok) { out.append(esc); break; }
				i += 4;
				// high surrogate: combine with following low surrogate
				if (cp >= 0xD800 && cp <= 0xDBFF
					&& i + 6 < n && s[i+1] == '\\' && s[i+2] == 'u') {
					unsigned int low = 0;
					bool lok = true;
					for (int j = 3; j <= 6; j++) {
						int v = hexDigit(s[i + j]);
						if (v < 0) { lok = false; break; }
						low = (low << 4) | (unsigned int)v;
					}
					if (lok && low >= 0xDC00 && low <= 0xDFFF) {
						cp = 0x10000 + ((cp - 0xD800) << 10) + (low - 0xDC00);
						i += 6;
					}
				}
				appendUTF8(out, cp);
				break;
			}
			default:
				out.append(esc);
				break;
		}
	}
}

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
	if (this->lexer.readIdentifier()) {
		*key = this->lexer.getTokenInputString();
		return true;
	}
	if (this->lexer.readString()) {
		unescapeJSONString(this->lexer.getTokenInputStringWithoutQuotations(), *key);
		return true;
	}
	return false;
}

bool JSONReader::readValue(JSValue& value) {
  JSObject* obj = NULL;
  std::vector<JSValue>* list = NULL;
  
  // string
  if (this->lexer.readString()) {
		value.str = new string();
		unescapeJSONString(lexer.getTokenInputStringWithoutQuotations(), *value.str);
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

}