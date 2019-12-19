///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef lexer_h
#define lexer_h

#include <stdio.h>

#include "string.h"
#include "stringstream.h"
#include "types.h"

namespace ucm {
	
	enum Operators {
		op_assign,
		op_add,
		op_sub,
		op_mul,
		op_div,
		op_mod,
		op_shift_left,
		op_shift_right,
		op_logic_and,
		op_logic_or,
		op_binary_and,
		op_binary_or,
		op_binary_xor,
		op_unary_minus,
		op_unary_not,
		op_unary_pre_add,
		op_unary_pre_sub,
		op_unary_post_add,
		op_unary_post_sub,
	};

	enum TokenType {
		token_nil,
		token_identifier,
		token_op,
		token_number,
		token_string,
		token_boolean,
		token_keyword,
	};

	struct Token {
		TokenType type = TokenType::token_nil;
		
		uint start = 0;
		uint length = 0;
		
		union
		{
			double v_num;
			bool v_bool;
			Operators v_op;
		};
	};

	class Lexer 
	{
	private:
		StringReader stream;
		char c = STR_EOF;
		int pos = -1;
		Token currentToken;
		string currentTokenInputString;
		string currentTokenInputStringWithoutQuotations;
		bool tokenInputStringChanged = false;
		
		inline void nextChar() {
			c = stream.readChar();
			pos++;
		}

		void setToken(const TokenType type, const int start, const int length);
		void prepareTokenInputStrings();
		
	public:
		Lexer() { }
		Lexer(const string& input);
		
		virtual void setInput(const string& input);
		const string& getInput() const;

		inline char getCurrentChar() const { return this->c; }

		inline bool eof() const {
			return this->c == STR_EOF || this->pos >= stream.getLength();
		}
		
		bool enableSkipWS = true;
		
		inline const Token& getCurrentToken() { return this->currentToken; }
		
		void skipWS();

		bool readIdentifier();
		bool readOperator();
		bool readNumber();
		bool readString();
		bool readBoolean();
		bool readChar(const char c);
		bool readKeyword(const char** dict);
		bool readCharRange(const char from, const char to);
		bool readAlphabetNumberAndValidSymbols();
		bool readAlphabetAndNumberChar();
		
		int getTokenInput(char* buffer) const;
		const string& getTokenInputString();
		const string& getTokenInputStringWithoutQuotations();
	};
}

#endif /* lexer_h */
