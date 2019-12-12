///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef USE_LOWER_NULL
#define null NULL
#endif /* USE_LOWER_NULL */

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char byte;

#endif // __TYPES_H__