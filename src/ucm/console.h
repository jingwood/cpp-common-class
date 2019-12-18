///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef console_h
#define console_h

#include "types.h"
#include "string.h"

#define DEC_OUT_FUNC(NAME) void NAME(const string& str); void NAME(const char* fmt, ...)

namespace unvell_common_class {
	
	enum VerboseLevel {
		VL0_Nothing,
		VL1_Error,
		VL2_Warning,
		VL3_Info,
		VL4_Trace,
		VL5_Debug,
	};

	class Console {
	private:
		byte verboseLevel = VerboseLevel::VL0_Nothing;
		
	public:	
		inline void setLevel(const VerboseLevel level) { this->verboseLevel = level; }
		
		void write(const string& str) const;
		void write(const char* fmt, ...) const;
		
		DEC_OUT_FUNC(error);
		DEC_OUT_FUNC(warn);
		DEC_OUT_FUNC(info);
		DEC_OUT_FUNC(trace);
		DEC_OUT_FUNC(debug);
	};

}

#undef DEC_OUT_FUNC

#endif /* console_h */
