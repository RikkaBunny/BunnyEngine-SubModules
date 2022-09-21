#pragma once

#ifdef BE_PLATFORM_WINDOWS
	#ifdef BE_BUILD_DLL
		#define BE_API __declspec(dllexport)
	#else
		#define BE_API __declspec(dllimport)
	#endif // BE_BUILD_DLL
#else 
	#error BunnyEngine only support Windows!
#endif
