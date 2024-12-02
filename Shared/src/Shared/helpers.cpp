#include "helpers.h"

#ifdef _DEBUG
#include <cstdio>
#endif

namespace debug {
	void printf(const char* format, ...) {
		#ifdef _DEBUG
		::printf(format);  // NOLINT(clang-diagnostic-format-security)
		#endif
	}
	void printf(const wchar_t* format...) {
		#ifdef _DEBUG
		::wprintf(format);  // NOLINT(clang-diagnostic-format-security)
		#endif
	}
}

