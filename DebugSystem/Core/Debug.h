#pragma once

#pragma warning(disable:4251)


#ifdef DEBUG_BUILD_DLL
#define JX_DEBUG_API	__declspec(dllexport)
#else
#define JX_DEBUG_API	__declspec(dllimport)
#endif