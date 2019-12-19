///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef hashtable_h
#define hashtable_h

#include <stdio.h>
#include <map>

#include "list.h"
#include "strutil.h"

namespace ucm {

typedef unsigned int uint;

template<typename T>
struct dictlist {
  std::map<const char*, T>* list = NULL;
  T value;
};

template<typename T>
class dict {
private:
  std::map<uint, dictlist<T>> items;
  
public:
  void set(const char* key, const T& value)
  {
    uint k = hash(key);
    
    auto list = items.find(k);
    
    if (list == items.end())
    {
      dictlist<T> node;
      node.value = value;
      return;
    }
    
    if (list->second.list != NULL) {
    }
  }
  
  T get(const char* key)
  {
    uint k = hash(key);
    
    std::map<const char*, T>* cands = items.find(k);
    
    if (cands != NULL)
    {
      for (auto& it = cands->begin(); it != cands->end(); it++) {
        if (strcmp(it.first, key) == 0) {
          return it.second;
        }
      }
    }
    
    return NULL;
  }
  
//  bool contains(const char* key)
//  {
//    uint k = hash(key);
//    
//    std::map<const char*, T>* cands = items.find(k);
//
//    if (cands != NULL)
//    {
//      for (auto& it = cands->begin(); it != cands->end(); it++) {
//        if (strcmp(it.first, key) == 0) {
//          return true;
//        }
//      }
//    }
//
//    return false;
//  }
};

}

#endif /* hashtable_h */
