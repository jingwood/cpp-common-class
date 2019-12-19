///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef regex_h
#define regex_h

#if defined(_WIN32)
#pragma once
#endif /* _WIN32 */

#include <stdio.h>
#include <vector>
#include "types.h"
#include "string.h"
#include "stringstream.h"
#include "exception.h"

namespace ucm {

	class Match;

	class Regex {
	private:
		
		enum PatternTokenType {
			RTT_LeftGroup,
			RTT_RightGroup,
			RTT_LeftRange,
			RTT_RightRange,
			RTT_LeftCatch,
			RTT_RightCatch,

			RTT_String,
			RTT_CharRange,
			RTT_Word_Number,
			RTT_Word_Lower,
			RTT_Word_Upper,
		};
		
		struct PatternToken {
			uint start;
			uint length;
			PatternTokenType type;
		};
		
		class PatternLexer {
		private:
			const string* input = NULL;
			char c = STR_EOF;
			uint pos = 0;
			uint tokenStart = 0;
			PatternToken currentToken;
			
			void nextChar();
			void setToken(const PatternTokenType type);
			
		public:
			PatternLexer();
			PatternLexer(const string& input);
			
			void setInput(const string& input);
			bool nextToken();
			
			const PatternToken& getCurrentToken() const {
				return this->currentToken;
			}
		};
		
		StringReader input;
		char c = 0;
		void nextChar();

		enum ElementOp {
			EOP_Read_Char,
			EOP_Read_CharRange,
			EOP_Read_String,
			EOP_Read_Any,

			EOP_Repeat,

			EOP_Logic_Or,

			EOP_Catch,
		};

		enum RepeatTimes {
			RT_One,
			RT_OneOrNothing,
			RT_Multiple,
		};

		struct Element;

		struct Element {
			ElementOp op;

			union {
				unsigned char readChar;

				//int repeatTimes;

				struct {
					short minRepeatTimes;
					short maxRepeatTimes;
				};

				struct {
					ushort fromChar;
					ushort toChar;
				};

				const string* string = NULL;
			};

			std::vector<Element> children;
		};

		std::vector<Element> elementTree;

		std::vector<string> refStrings;
		const string* newRefString(const string& str);

		bool matchInput(const string& str, std::vector<Match>& matches);
		bool processElements(const std::vector<Element>& elementTree, std::vector<Match>& matches);
		bool processElement(const Element& element, std::vector<Match>& matches);

		static inline Element e_makeRepeatNoneOrMultiple(const Element& inner) {
			Element eoutter;
			eoutter.op = EOP_Repeat;
			eoutter.minRepeatTimes = 0;
			eoutter.maxRepeatTimes = 0;
			
			eoutter.children.push_back(inner);

			return eoutter;
		}

		static inline Element e_makeCatch(const Element& e) {
			Element ecatch;
			ecatch.op = EOP_Catch;
			ecatch.children.push_back(e);
			return ecatch;
		}

		static inline Element e_makeCatch(const std::vector<Element>& es) {
			Element ecatch;
			ecatch.op = EOP_Catch;
			for (const Element& e : es) {
				ecatch.children.push_back(e);
			}
			return ecatch;
		}

		static inline Element e_makeReadCharRange(const char from, const char to) {
			Element e;
			e.op = EOP_Read_CharRange;
			e.fromChar = from;
			e.toChar = to;
			return e;
		}

	public:
		Regex();

		void compile(const string& pattern);

		bool matches(const string& str, int* matchCount = NULL);
		bool matches(const string& str, std::vector<Match>& matches);
	};

	class Match {
		friend Regex;

	private:
		uint position;
		uint length;

	public:
		inline const uint getPosition() const {
			return this->position;
		}

		inline const uint getLength() const {
			return this->length;
		}
	};

	class InvalidRegexPatternException : public Exception {
	};

}
#endif /* regex_h */

