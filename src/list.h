///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef list_h
#define list_h

#include <stdio.h>
#include <memory>
#include <vector>
#include "string.h"

#define LIST_INIT_CAP 128

namespace ccm {
	template <typename T>
	class List
	{
	private:
		const T* items;
		int count;
		int size;
		
	public:
		List()
		{
			this->items = new T[LIST_INIT_CAP];
		}
		
		~List()
		{
			delete [] this->items;
			this->items = NULL;
		}
		
		void add(const T& t) {
			if (this->count >= this->size)
			{
				T* newitems = new T[this->size + LIST_INIT_CAP];
				memcpy(newitems, this->items, this->count);
				delete this->items;
				this->items = newitems;
			}
			
			this->items[this->count] = t;
			this->count++;
		}
		
		inline T get(int i) {
			return this->items[i];
		}
		
		inline int getCount() const {
			return this->count;
		}
	};

	inline bool list_contains_substr(const std::vector<string>& list, const string& str) {
		for (const string& item : list) {
			if (item.contains(str)) {
				return true;
			}
		}

		return false;
	}

	inline bool list_contains_substr(const std::vector<string>& list, const std::vector<string>& str) {
		for (const string& item : list) {
			for (const string& s : str) {
				if (item.contains(s)) {
					return true;
				}
			}
		}
		
		return false;
	}
}

#endif /* list_h */

