///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "lexer.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define set_start     uint __start = pos
#define set_length    this->currentToken.start = __start; \
											this->currentToken.length = this->stream.getPosition() - __start - 1

namespace ccm {

	Lexer::Lexer(const string& input) {
		this->setInput(input);
	}

	void Lexer::setInput(const string& input) {
		this->stream.setInput(input);
		this->pos = -1;
		
		this->nextChar();
	}

	const string& Lexer::getInput() const {
		return this->stream.getInput();
	}

	void Lexer::setToken(const TokenType type, const int start, const int length) {
		this->currentToken.type = type;
		this->currentToken.start = start;
		this->currentToken.length = length;
		
		this->tokenInputStringChanged = true;
	}

//	int Lexer::getTokenInput(char *buffer) const {
//		const Token& t = this->currentToken;
//		
//		memcpy(buffer, this->stream.getBuffer() + t.start, t.length);
//
//		return t.length;
//	}

	void Lexer::prepareTokenInputStrings() {
		if (!this->tokenInputStringChanged) {
			const Token& t = this->currentToken;
			this->currentTokenInputString.clear();
			this->currentTokenInputString.append(this->getInput() + t.start, t.length);

			this->currentTokenInputStringWithoutQuotations.clear();
			this->currentTokenInputStringWithoutQuotations.append(this->getInput() + t.start + 1, t.length - 2);
			this->tokenInputStringChanged = false;
		}
	}

	const string& Lexer::getTokenInputString() {
		this->prepareTokenInputStrings();
		return this->currentTokenInputString;
	}

	const string& Lexer::getTokenInputStringWithoutQuotations() {
		this->prepareTokenInputStrings();
		return this->currentTokenInputStringWithoutQuotations;
	}

	void Lexer::skipWS() {
		if (!enableSkipWS) return;
		
		bool inLineComment = false;
		bool inBlockComment = false;
		
		while (c != STR_EOF) {
			if (inBlockComment) {
				if (c == '*') {
					nextChar();
					
					if (c == STR_EOF) {
						return;
					}
					else if (c == '/') {
						inBlockComment = false;
						nextChar();
						continue;
					}
				} else {
					nextChar();
					continue;
				}
			}
			else if (inLineComment) {
				if (c == '\n') {
					inLineComment = false;
				}
				else if (c == '\r')
				{
					inLineComment = false;
					if (c == '\n') nextChar();
				}

				nextChar();
				continue;
			}
			
			switch (c) {
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					nextChar();
					continue;
					
				case '/':
					nextChar();
					
					switch (c)
					{
						case STR_EOF: return;
						case '*': inBlockComment = true; nextChar(); continue;
						case '/': inLineComment = true; nextChar(); continue;
					}

				default:
					return;
			}
		}
	}

	bool Lexer::readIdentifier() {
		skipWS();
		
		bool first = true;
		uint length = 0;
		set_start;
		
		while (true)
		{
			if ((c >= 'a' && c <= 'z')
					|| (c >= 'A' && c <= 'Z')
					|| c == '_' || c == '$')
			{
				nextChar();
				length++;
				first = false;
			}
			else if(!first
							&& c >= '0' && c <= '9')
			{
				nextChar();
				length++;
			} else {
				break;
			}
		}
		
		if (length > 0)
		{
			this->currentToken.type = TokenType::token_identifier;
			this->currentToken.start = __start;
			this->currentToken.length = length;
			
			return true;
		}
		else
			return false;
	}

	bool Lexer::readOperator()
	{
		skipWS();
		
		currentToken.type = TokenType::token_op;
		set_start;
		
		switch (c)
		{
			case '+':
				nextChar();
				currentToken.v_op = Operators::op_add;
				set_length;
				return true;
				
			case '-':
				nextChar();
				currentToken.v_op = Operators::op_sub;
				set_length;
				return true;
				
			case '=':
			{
				nextChar();
				
				switch (c)
				{
					default:
						currentToken.v_op = Operators::op_assign;
						set_length;
						return true;
				}
			}
				
			default:
				return false;
		}
	}

	bool Lexer::readNumber() {
		skipWS();
		
		if (c == STR_EOF) {
			return false;
		}
		
		double value = 0;
		bool negative = false;
		
		set_start;
		
		if (c == '-') {
			negative = true;
			nextChar();
			
			if (c == STR_EOF) {
				return false;
			}
		}
		
		bool exponentPart = false;
		int exponentValue = 0;
		bool minusExponent = false;
		
		double digits = 0;
		
		while (true) {
			if (c >= '0' && c <= '9') {
				int v = (c - '0');
				
				if (exponentPart) {
					exponentValue = exponentValue * 10 + v;
				} else {
					if (digits > 0) {
						value = value + v / pow(10, digits);
						digits++;
					}
					else {
						value = value * 10 + v;
					}
				}
				
				nextChar();
			}
			else if (c == '.') {
				if (digits != 0) return false;

				digits = 1;
				nextChar();
			}
			else if (c == 'e') {
				if (exponentPart) return false;

				exponentPart = true;
				nextChar();
			}
			else if (c == '-') {
				if (!exponentPart || minusExponent) return false;
				
				minusExponent = true;
				nextChar();
			}
			else
				break;
		}
		
		int length = this->pos - __start;
		
		if (length <= 0) {
			return false;
		}
		
		if (exponentPart) {
			if (minusExponent) exponentValue = -exponentValue;
			value = value * pow(10, exponentValue);
		}
		
		currentToken.type = TokenType::token_number;
		currentToken.v_num = (negative ? -value : value);
		currentToken.start = __start;
		currentToken.length = (uint)length;
		
		return true;
	}

	bool Lexer::readString() {
		skipWS();
		
		set_start;
		
		if (c != '\'' && c != '"') {
			return false;
		}
		
		const char endChar = c;
		
		bool inEscape = false;
		
		while (true) {
			if (c == STR_EOF) {
				return false;
			}
			
			if (c == '\\') {
				inEscape = true;
			} else {
				inEscape = false;
			}
			
			nextChar();
			
			if (c == endChar && !inEscape) {
				nextChar();
				break;
			}
		}
		
		currentToken.type = TokenType::token_string;

		set_length;
		
		return true;
	}

	static const char validBooleanChars[] = "turefals";
	static constexpr int validBooleanCharsLen = 8;

	inline bool isValidBooleanChar(char c) {
		for (int i = 0; i < validBooleanCharsLen; i++) {
			if (c == validBooleanChars[i]) return true;
		}
		return false;
	}

	bool Lexer::readBoolean() {
		skipWS();
		
		set_start;
		
		while (true) {
			if (isValidBooleanChar(c)) {
				nextChar();
			} else {
				break;
			}
		}

		bool success = false;
		
		if (strncmp(stream.getInput().getBuffer() + __start, "true", 4) == 0) {
			currentToken.v_bool = true;
			success = true;
		} else if (strncmp(stream.getInput().getBuffer() + __start, "false", 5) == 0) {
			currentToken.v_bool = false;
			success = true;
		}
		
		if (success) {
			currentToken.type = TokenType::token_boolean;
			set_length;
			return true;
		}

		return false;
	}

	bool Lexer::readKeyword(const char** dict)
	{
		return false;
		
	//	skipWS();
	//	
	//	set_start;
	//	
	//	
	//	currentToken.type = TokenTypes::token_keyword;
	//	
	//	set_length;
	//	
	//  return true;
	}

	bool Lexer::readChar(const char c) {
		this->skipWS();
		
		if (this->c == c) {
			this->nextChar();
			return true;
		}
		
		return false;
	}

	bool Lexer::readCharRange(const char from, const char to) {
		if (this->c >= from && this->c <= to) {
			this->nextChar();
			return true;
		}
		
		return false;
	}
	
	bool Lexer::readAlphabetNumberAndValidSymbols() {
		
		this->skipWS();
		
		uint start = this->pos;
		uint len = 0;

		while (!eof()) {
			if (!this->readAlphabetAndNumberChar()
					&& !this->readChar('_') && !this->readChar('-')
					&& !this->readChar('$')) {
				break;
			}
			len++;
		}
		
		if (len > 0) {
			this->currentToken.type = TokenType::token_identifier;
			this->currentToken.start = start;
			this->currentToken.length = len;
			return true;
		}
		
		return false;
	}
	
	bool Lexer::readAlphabetAndNumberChar() {
		return this->readCharRange('a', 'z') || this->readCharRange('A', 'Z')
		|| this->readCharRange('0', '9');
	}

}
