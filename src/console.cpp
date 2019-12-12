///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "console.h"
#include <stdarg.h>

#define STDCHOOSE(E) (E ? stderr : stdout)

#define VL(LEVEL)	(this->verboseLevel >= (LEVEL))

#define DEC_CONSOLE_OUTPUT_FUNC(NAME, ISERR, LEVEL) \
	void Console::NAME(const string& str) { \
		if (VL(LEVEL)) { \
			fprintf(STDCHOOSE(ISERR), "%s", str.getBuffer()); \
		} \
	} \
	void Console::NAME(const char* fmt, ...) { \
		if (VL(LEVEL)) {\
			va_list ap; \
			va_start(ap, fmt); \
			vfprintf(STDCHOOSE(ISERR), fmt, ap); \
			va_end(ap); \
		} \
	}

namespace ccm {
	
	void Console::write(const string& str) const {
		printf("%s", str.getBuffer());
	}
	
	void Console::write(const char* fmt, ...) const {
		va_list ap;
		va_start(ap, fmt);
		vfprintf(STDCHOOSE(false), fmt, ap);
		va_end(ap);
	}

	DEC_CONSOLE_OUTPUT_FUNC(error, true,  VerboseLevel::VL1_Error  );
	DEC_CONSOLE_OUTPUT_FUNC(warn,  false, VerboseLevel::VL2_Warning);
	DEC_CONSOLE_OUTPUT_FUNC(info,  false, VerboseLevel::VL3_Info   );
	DEC_CONSOLE_OUTPUT_FUNC(trace, false, VerboseLevel::VL4_Trace  );
	DEC_CONSOLE_OUTPUT_FUNC(debug, false, VerboseLevel::VL5_Debug  );

}

#undef DEC_CONSOLE_OUTPUT_FUNC
#undef VL
#undef STDCHOOSE
