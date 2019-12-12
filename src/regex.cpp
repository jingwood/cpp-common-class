///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "regex.h"

namespace ccm {
	Regex::PatternLexer::PatternLexer() {
		
	}
	
	Regex::PatternLexer::PatternLexer(const string& input) {
		this->setInput(input);
	}
	
	void Regex::PatternLexer::setInput(const string& input) {
		this->input = &input;
		this->nextChar();
	}
	
	void Regex::PatternLexer::nextChar() {
		this->c = (*this->input)[this->pos];
		this->pos++;
	}
	
	bool Regex::PatternLexer::nextToken() {
		const int len = this->input->length();
		
		if (this->pos > len) {
			return false;
		}
		
		switch (c) {
			default:
				while (this->pos < len) {
					const char nc = (*this->input)[this->pos + 1];
					if (nc == '(' || c == ')' || c == '\\') {
						break;
					}
					nextChar();
				}
				setToken(RTT_String);
				return true;
				
			case '(': setToken(PatternTokenType::RTT_LeftCatch); break;
			case ')': setToken(PatternTokenType::RTT_RightCatch); break;
			case '-': setToken(PatternTokenType::RTT_CharRange); break;

			case '\\':
				if (this->pos + 1 >= this->input->length())
					throw InvalidRegexPatternException();
				
				nextChar();
				
				switch (c) {
					default: throw InvalidRegexPatternException();
						
					case 'd': setToken(RTT_Word_Number); break;
					case 'w': setToken(RTT_Word_Lower); break;
					case 'W': setToken(RTT_Word_Upper); break;
				}
				
				break;
		}
		
		return true;
	}
								 
	void Regex::PatternLexer::setToken(const ccm::Regex::PatternTokenType type) {
		this->currentToken.start = this->tokenStart;
		this->currentToken.length = this->pos - this->tokenStart;
		this->currentToken.type = type;
		this->tokenStart = this->pos;

		nextChar();
	}
	
	//Regex::Regex::Element::~Element() {
	//	if (this->op == EOP_Read_String && this->data.string != NULL) {
	//		delete this->data.string;
	//		this->data.string = NULL;
	//	}
	//}

	Regex::Regex() {

		//Element e_number;
		//e_number.op = EOP_Read_CharRange;
		//e_number.fromChar = 48;
		//e_number.toChar = 57;

		//Element e2;
		//e2.op = EOP_Repeat;
		//e2.minRepeatTimes = 3;
		//e2.maxRepeatTimes = 3;
		//e2.children.push_back(e_number);

		//Element e3;
		//e3.op = EOP_Read_Char;
		//e3.readChar = '-';

		//Element e4;
		//e4.op = EOP_Repeat;
		//e4.minRepeatTimes = 4;
		//e4.maxRepeatTimes = 4;

		//e4.children.push_back(e_number);

		//Element ecatch;
		//ecatch.op = EOP_Catch;
		//
		//ecatch.children.push_back(e2);
		//ecatch.children.push_back(e3);
		//ecatch.children.push_back(e4);

		//Element e1;
		//e1.op = EOP_Read_String;
		//e1.data.string = this->newRefString("http");

		//Element e2;
		//e2.op = EOP_Read_Any;
		//e2 = makeRepeatNoneOrMultiple(e2);

		//Element ecatch;
		//ecatch.op = EOP_Catch;
		////ecatch.children.push_back(e1);
		//ecatch.children.push_back(e2);

		//this->elementTree.push_back(ecatch);
	}

	const string* Regex::newRefString(const string& str) {
		this->refStrings.push_back(str);
		return &this->refStrings.back();
	}

	void Regex::compile(const string& pattern) {
		PatternLexer lexer(pattern);
		
		while (lexer.nextToken()) {
			auto& token = lexer.getCurrentToken();
			string str;
			pattern.substring(token.start, token.length, str);
			printf("-> %s\n", str.getBuffer());
		}
		
		return;
		
		StringReader reader(pattern);
		
		std::vector<Element> stack;

		do {
			char c = reader.readChar();

			switch (c) {
			default:
			{
				Element* estr = NULL;

				if (!stack.empty()) {
					Element& last = stack.back();

					if (last.op == EOP_Read_String) {
						estr = &last;
					}
				}

				if (estr == NULL) {
					Element enewstr;
					estr = &enewstr;
					estr->op = EOP_Read_String;
					estr->string = this->newRefString(string::empty);
					stack.push_back(*estr);
				}

				(const_cast<string*>(estr->string))->append(c);
			}
			break;

			case '\\':
				if (this->input.isEnd()) {
					throw InvalidRegexPatternException();
				}

				c = reader.readChar();
				
				switch (c) {
				default:
					throw InvalidRegexPatternException();

				case 'd':
					stack.push_back(e_makeReadCharRange('0', '9'));
					break;

				case 'w':
					stack.push_back(e_makeReadCharRange('a', 'z'));
					break;

				case 'W':
					stack.push_back(e_makeReadCharRange('A', 'Z'));
					break;
				}
			}

		} while (!reader.isEnd());

		while (!stack.empty()) {
			const Element& e = stack.back();
			stack.pop_back();
			this->elementTree.push_back(e);
		}

		Element ecatch = e_makeCatch(this->elementTree);
		this->elementTree.clear();
		this->elementTree.push_back(ecatch);
	}

	void Regex::nextChar() {
		this->c = this->input.readChar();
	}

	bool Regex::matches(const string& str, int* matchCount) {
		std::vector<Match> matches;
		bool success = this->matchInput(str, matches);
		if (matchCount != NULL) {
			*matchCount = (int)matches.size();
		}
		return success;
	}

	bool Regex::matches(const string& str, std::vector<Match>& matches) {
		return this->matchInput(str, matches);
	}

	bool Regex::matchInput(const string& str, std::vector<Match>& matches) {
		this->input.setInput(str);
		this->nextChar();

		bool success = true;

		while (this->c != STR_EOF && success) {
			success = this->processElements(this->elementTree, matches);
		}

		return success;
	}

	bool Regex::processElements(const std::vector<Element>& elementTree, std::vector<Match>& matches) {
		bool success = true;

		for (const Element& e : elementTree) {
			if (!this->processElement(e, matches)) {
				success = false;
				break;
			}
		}

		return success;
	}

	bool Regex::processElement(const Element& element, std::vector<Match>& matches) {
		switch (element.op) {
			default: // TODO
				return false;
				
		case EOP_Read_Char:
			if (this->c == element.readChar) {
				this->nextChar();
				return true;
			}
			return false;

		case EOP_Read_CharRange:
			if (this->c >= element.fromChar && this->c <= element.toChar) {
				this->nextChar();
				return true;
			}
			return false;
			
		case EOP_Read_String:
			{
				if (element.string == NULL) return false;

				const char* cs = element.string->getBuffer();

				for (int i = 0; i < element.string->length(); i++) {
					if (this->c != *cs) {
						return false;
					}

					cs++;
					this->nextChar();
				}

				return true;
			}
			break;

		case EOP_Read_Any:
			if (!this->input.isEnd()) {
				this->nextChar();
				return true;
			}
			return false;

		case EOP_Repeat:
			{
				int i = 0;
				bool success = true;

				while (success) {
					bool test = this->processElements(element.children, matches);

					if (!test) {
						if (i >= element.minRepeatTimes) {
							break;
						}
					}
					
					success = test;
					
					if (success) {
						i++;

						if (element.maxRepeatTimes > 0 && i >= element.maxRepeatTimes) {
							break;
						}
					}
				}

				return success && i >= element.minRepeatTimes;
			}
			break;

		case EOP_Catch:
			{
				Match match;
				match.position = this->input.getPosition() - 1;
				bool success = this->processElements(element.children, matches);
				if (success) {
					match.length = this->input.getPosition() - match.position;
					matches.push_back(match);
				}
				return success;
			}
			break;
		}

		return false;
	}

}
