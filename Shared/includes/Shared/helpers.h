#pragma once

#include "library.h"

template<class Base>
class SHARED_API Singleton {
	friend Base;

	protected:
	Singleton() = default;


	public:
	Singleton(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	static Base& Get() {
		static Base s_instance;
		return s_instance;
	}
};

namespace debug {
	void SHARED_API printf(const char* format, ...);
	void SHARED_API printf(const wchar_t* format...);
}