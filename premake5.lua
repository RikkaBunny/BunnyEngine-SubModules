workspace "BunnyEngine"
	architecture "x64"


	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "BunnyEngine"
	location "BunnyEngine"
	kind "SharedLib"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "BEpch.h"
	pchsource "BunnyEngine/src/BEpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/SDK/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"BE_PLATFORM_WINDOWS",
			"BE_BUILD_DLL"
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}



	filter "configurations:Debug"
		defines "BE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "BE_DIST"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs{
		"BunnyEngine/SDK/spdlog/include",
		"BunnyEngine/src"
	}

	links{
		"BunnyEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"BE_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "BE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "BE_DIST"
		optimize "On"