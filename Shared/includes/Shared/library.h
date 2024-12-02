#pragma once

#ifdef SharedLib_EXPORTS
#define SHARED_API __declspec(dllexport)
#else
#define SHARED_API
#endif
