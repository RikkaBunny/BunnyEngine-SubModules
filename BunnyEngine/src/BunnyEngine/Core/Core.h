#pragma once

#include <memory>

#ifdef BE_PLATFORM_WINDOWS
#if BE_DYNAMIC_LINK
	#ifdef BE_BUILD_DLL
		#define BE_API __declspec(dllexport)
	#else
		#define BE_API __declspec(dllimport)
	#endif // BE_BUILD_DLL
#else
	#define BE_API
#endif
#else 
	#error BunnyEngine only support Windows!
#endif

#ifdef BE_ENABLE_ASSERTS
	#define BE_ASSERT(x,...) {if(!(x)) { BE_ERROR("Assertion Failed: {0}", __VA_VRGS__); __debugbreak();} }
	#define BE_CORE_ASSERT(x,...) { if(!(x)) { BE_CORE_ERROR("Assertion Failed: {0}", __VA_VRGS__); __debugbreak();} }
#else
	#define BE_ASSERT(x,...)
	#define BE_CORE_ASSERT(x,...)
#endif

#define BIT(x) (1 << x)

#define BE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace BE {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;


}
