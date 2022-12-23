workspace "BunnyEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "BunnyEngine/SDK/GLFW/include"
IncludeDir["Glad"] = "BunnyEngine/SDK/Glad/include"
IncludeDir["ImGui"] = "BunnyEngine/SDK/imgui"
IncludeDir["glm"] = "BunnyEngine/SDK/glm"

include "BunnyEngine/SDK/GLFW"
include "BunnyEngine/SDK/Glad"
include "BunnyEngine/SDK/imgui"

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
		"%{prj.name}/SDK/glm/glm/**.hpp",
		"%{prj.name}/SDK/glm/glm/**.inl"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/SDK/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"BE_PLATFORM_WINDOWS",
			"BE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}



	filter "configurations:Debug"
		defines "BE_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "BE_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "BE_DIST"
		buildoptions "/MD"
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
		"BunnyEngine/src",
		"%{IncludeDir.glm}"
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
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "BE_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "BE_DIST"
		buildoptions "/MD"
		optimize "On"