///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef sort_h
#define sort_h

#include <stdio.h>

template<typename T>
inline void swapItem(T& a, T& b) {
  const T tmp = a;
  a = b;
  b = tmp;
}

template <typename T, typename Fun>
void sort(T* arr, int count, Fun comparer) {

	for (int j = 0; j < count; j++) {
		for (int i = 1; i < count; i++) {
			T& a = arr[i - 1];
			T& b = arr[i];
			
			if (comparer(a, b)) {
				swapItem(a, b);
			}
		}
	}

	return;
	
//  int n = count;
//	
//  do {
//    int newn = 0;
//    
//    for (int i = 1; i < n - 1; i++) {
//      T& a = arr[i - 1];
//      T& b = arr[i];
//      
//      if (comparer(a, b)) {
//        swapItem(a, b);
//        newn = i;
//      }
//    }
//    
//    n = newn;
//    
//  } while (n > 0);
}

#endif /* sort_h */
